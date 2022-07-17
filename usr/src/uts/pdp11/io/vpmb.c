/*Version 2*/
#include "sys/param.h"
#include "sys/tty.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/systm.h"
#include "sys/uba.h"
#include "sys/vpm.h"
#include "sys/vpmd.h"


#define BISYNC	6		/*BISYNC interpreter*/
#define VPMBS	4		/*Number of Virtual Protocol Machines*/
#define BUFSIZE	512		/*Buffer size*/
#define NOARG	0		/*Null argument*/
#define VPMPRI	(PZERO+5)
#define VPMVER2	1

/*vpm state bits*/

#define OPEN	01
#define WCLOSE	02
#define VPMERR	04
#define VPMOK	010
#define VPMRPTA	020
#define VPMOKCK	040
#define VPMECNT	0100

/*Interpreter Options*/

#define INTROP0	(01<<0)

/*Per vmp structure*/

struct vpmb {
	struct	clist	vb_xmtq;	/*Xmit queue*/
	struct	clist	vb_emptq;	/*Empty queue*/
	int	(*vb_rint)();		/*Called upon receive interupt*/
	short	vb_state;		/*Status info*/
	char	vb_xbkmc;		/*Number of xmit buffers kmc has*/
	char	vb_xmax;		/*Maximum xmit buffers kmc can take*/
	char	vb_rbkmc;		/*Number of rcv buffers kmc has*/
	char	vb_rmax;		/*Maximum rcv buffers kmc can take*/
	char	vb_rpt[4];
	char	vb_errs[8];
} vpmb[VPMBS];

extern struct vpmbd *vpmdeq();

/*
 * Vpmstart is called by the top half of driver once for each kmc-line 
 * combination. Dev has the format kklllvvv; where kk is the kmc no. 
 * (0-3); lll is the line no. (0-7); and vvv is the logical vpm no. 
 * (0-7). Type is a constant indicating the script running in the 
 * kmc. Rint is the address of the routine to be called when a 
 * receive buffer or an error report is to be delevered to the top 
 * half. 
 */
vpmstart(dev,type,rint)
int (*rint)();
{
	register struct vpmb  *vp;
	extern vpmbrint();
	extern vpmok();

	if(BDEV(dev) >= VPMBS)
		return(ENXIO);
	if((vp = &vpmb[BDEV(dev)])->vb_state&OPEN)
		return(EACCES);
	if(kmcset(dev,type,vpmbrint))
		return(EACCES);
	vp->vb_rint = rint;
	vp->vb_state |= OPEN;
	vp->vb_xmax = 1;
	vp->vb_rmax = 1;
	kmcload(dev,RUNCMD,VPMVER2,NOARG);
	timeout(vpmok,dev,5*HZ);
	vpmsave('s',dev,vp->vb_state,0);
	return(0);
}
/*
 * Vpmstop is called by the top half of the driver when the last 
 * process using the kmc-line combination closes the top half. It 
 * halts the kmc and clears the vpmb structure. 
 */
vpmstop(dev)
{
	register struct vpmb *vp;
	extern vpmclean();
	extern vpmwake();

	vp = &vpmb[BDEV(dev)];
	vp->vb_state |= WCLOSE;
	timeout(vpmwake,dev,5*HZ);
	while(vp->vb_xbkmc)
		sleep((caddr_t)&vp->vb_xbkmc, VPMPRI);
	timeout(vpmclean,dev,5*HZ);
	kmcload(dev,HALTCMD,NOARG,NOARG);
	vpmsave('t',dev,vp->vb_state,vp->vb_xbkmc);
}
/*
 * Vpmxmtq queues pointers to buffer descriptors for transmission. 
 * If the kmc currently does not have a xmit buffer, the buffer 
 * descriptor is given to the kmc; otherwise it is queued. The 
 * current queue level is returned. If bdp = 0, the current queue 
 * level is returned; nothing else is done. 
 */
vpmxmtq(dev,bdp)
struct vpmbd *bdp;
{
	register struct vpmb *vp;
	register int sps,ql;
	paddr_t	addr;

	vp = &vpmb[BDEV(dev)];
	sps = spl5();
	if(bdp == 0)
		ql = vp->vb_xmtq.c_cc/sizeof(struct vpmbd *) + vp->vb_xbkmc;
	else{
		vpmenq(bdp,&vp->vb_xmtq);
		if(vp->vb_xbkmc < vp->vb_xmax) {
			bdp = vpmdeq(&vp->vb_xmtq);
			addr = ubmdata(bdp);
			kmcload(dev,XBUFINCMD,loword(addr),hiword(addr));
			vp->vb_xbkmc++;
			ql = vp->vb_xmtq.c_cc/sizeof(struct vpmbd *) + vp->vb_xbkmc;
		}
	}
	splx(sps);
	return(ql);
}
/*
 * Vpmempt is called by the top half of the driver to queue empty 
 * buffers for use by the bottom half as receive buffers. At least 
 * one receive buffer should be available at all times. For bdp = 0, 
 * the queue level is returned; nothing is done. For bdp > 0, the 
 * pointer is queued and the queue level is returned. 
 */
vpmemptq(dev,bdp)
struct vpmbd *bdp;
{
	register struct vpmb *vp;
	register int sps,ql;
	paddr_t	addr;

	vp = &vpmb[BDEV(dev)];
	sps = spl5();
	if(bdp == 0)
		ql = vp->vb_emptq.c_cc/sizeof(struct vpmbd *) + vp->vb_rbkmc;
	else{
		vpmenq(bdp,&vp->vb_emptq);
		if(vp->vb_rbkmc < vp->vb_rmax) {
			bdp = vpmdeq(&vp->vb_emptq);
			addr = ubmdata(bdp);
			kmcload(dev,RBUFINCMD,loword(addr),hiword(addr));
			vp->vb_rbkmc++;
			ql = vp->vb_emptq.c_cc/sizeof(struct vpmbd *) + vp->vb_rbkmc;
		}
	}
	splx(sps);
	return(ql);
}
vpmbrint(dev,code,sel4,sel6)
{
	register struct vpmb *vp;
	register struct vpmbd *bdp;
	paddr_t	addr;

	vp = &vpmb[BDEV(dev)];
	switch(code) {
		case RRTNXBUF:
			vpmsave('X',dev,vp->vb_xbkmc,0);
			vp->vb_xbkmc--;
			if((vp->vb_state&WCLOSE) && (vp->vb_xbkmc == 0)) {
				wakeup((caddr_t)&vp->vb_xbkmc);
				return;
			}
			while(vp->vb_xbkmc < vp->vb_xmax && (bdp = vpmdeq(&vp->vb_xmtq))) {
				addr = ubmdata(bdp);
				kmcload(dev,XBUFINCMD,loword(addr),hiword(addr));
				vp->vb_xbkmc++;
			}
			(*vp->vb_rint)(dev,code,(struct vpmbd *)ubmrev(sel4,sel6));
			break;
		case RRTNRBUF:
			vpmsave('R',dev,vp->vb_rbkmc,0);
			(*vp->vb_rint)(dev,code,(struct vpmbd *)ubmrev(sel4,sel6));
			vp->vb_rbkmc--;
			while(vp->vb_rbkmc < vp->vb_rmax && (bdp = vpmdeq(&vp->vb_emptq))) {
				addr = ubmdata(bdp);
				kmcload(dev,RBUFINCMD,loword(addr),hiword(addr));
				vp->vb_rbkmc++;
			}
			break;
		case RTRACE:
			vpmsave('T',dev,sel4,sel6);
			break;
		case ERRTERM:
			vpmsave('E',dev,sel4,sel6);
			if((sel6&ERRMASK) == HALTRCVD)
				vpmclean(dev);
			else
				(*vp->vb_rint)(dev,code,sel6);
			break;
		case RTNOK:
			vp->vb_state |= VPMOK;
			kmcload(dev,OKCMD,NOARG,NOARG);
			break;
		case RTNSNAP:
			vpmsnap('S',dev,sel4,sel6);
			break;
		case RTNSRPT:
			vp->vb_rpt[0] = sel4&0377;
			vp->vb_rpt[1] = (sel4>>8)&0377;
			vp->vb_rpt[2] = sel6&0377;
			vp->vb_rpt[3] = (sel6>>8)&0377;
			vp->vb_state |= VPMRPTA;
			vpmsave('P',dev,sel4,sel6);
			break;
		case RTNERCNT:
			vpmsave('F',dev,sel4,sel6);
			vp->vb_state |= VPMECNT;
			wakeup((caddr_t)vp->vb_errs);
			break;
		case STARTUP:
			vp->vb_xmax = sel4&0377;
			vp->vb_rmax = (sel4>>8)&0377;
			if(sel6&INTROP0)
				vp->vb_state |= VPMOKCK;
			vpmsave('S',dev,sel4,sel6);
	}
}
vpmenq(bdp,clp)
struct vpmbd *bdp;
struct clist *clp;
{
	register char *p;
	register int n;

	if(bdp == 0)
		return(clp->c_cc/sizeof(struct vpmbd *));
	p = (char *)&bdp;
	for(n=sizeof(struct vpmbd *);--n>=0;)
		putc(*p++,clp);
	return(clp->c_cc/sizeof(struct vpmbd *));
}
struct vpmbd *
vpmdeq(clp)
struct clist *clp;
{
	struct vpmbd *bdp;
	register int n;
	register char *p;

	if(clp->c_cc == 0)
		return(0);
	p = (char *)&bdp;
	for(n=sizeof(struct vpmbd *);--n>=0;)
		*p++ = getc(clp);
	return(bdp);
}
vpmclean(dev)
{
	struct vpmb *vp;
	struct vpmbd *bdp;

	vp = &vpmb[BDEV(dev)];
	if((vp->vb_state&WCLOSE) == 0)
		return;
	vp->vb_state &= ~WCLOSE;
	while(bdp = vpmdeq(&vp->vb_xmtq))
		(*vp->vb_rint)(dev,RRTNEBUF,bdp);
	while(bdp = vpmdeq(&vp->vb_emptq))
		(*vp->vb_rint)(dev,RRTNEBUF,bdp);
	vpmsave('C',dev,vp->vb_state,vp->vb_xbkmc);
	vp->vb_xbkmc = 0;
	vp->vb_rbkmc = 0;
	vp->vb_state = 0;
}
vpmwake(dev)
{
	struct vpmb *vp;

	vp = &vpmb[BDEV(dev)];
	vp->vb_xbkmc = 0;
	wakeup((caddr_t)&vp->vb_xbkmc);
}
vpmcmd(dev,cmd)
char cmd[];
{
	short sel4,sel6;

	sel4 = ((short)cmd[1]<<8)|cmd[0];
	sel6 = ((short)cmd[3]<<8)|cmd[2];
	kmcload(dev,SCRIPTCMD,sel4,sel6);
}
char *
vpmrpt(dev)
{
	struct vpmb *vp;

	vp = &vpmb[BDEV(dev)];
	if(vp->vb_state&VPMRPTA) {
		vp->vb_state &= ~VPMRPTA;
		return(vp->vb_rpt);
	}
	return(0);
}
vpmok(dev)
{
	struct vpmb *vp;

	vp = &vpmb[BDEV(dev)];
	if((vp->vb_state&OPEN) == 0 || (vp->vb_state&VPMOKCK) == 0)
		return;
	if(vp->vb_state&VPMOK)
		vp->vb_state &= ~VPMOK;
	else{
		vpmsave('O',dev,vp->vb_state,0);
		(*vp->vb_rint)(dev,ERRTERM,NORESP);
	}
	timeout(vpmok,dev,5*HZ);
}
char *
vpmerrs(dev,n)
{
	struct vpmb *vp;
	paddr_t	addr;
 
	vp = &vpmb[BDEV(dev)];
	if(n <= 0 || n > 8)
		n = 8;
	vp->vb_state &= ~VPMECNT;
	addr = ubmdata(vp->vb_errs);
	kmcload(dev,GETECMD,loword(addr),(n<<8)|hiword(addr));
	spl5();
	while((vp->vb_state&VPMECNT) == 0)
		sleep((caddr_t)vp->vb_errs, VPMPRI);
	spl0();
	return(vp->vb_errs);
}
vpmsave(type,dev,word1,word2)
char type,dev;
short word1,word2;
{
	static int vpmseqn;
	register sps;
	struct {
		short	e_seqn;
		char	e_type;
		char	e_dev;
		short	e_word1;
		short	e_word2;
	} vpment;

	sps = spl5();
	if(vpmseqn >= 077777)
		vpmseqn = 0;
	vpment.e_seqn = ++vpmseqn;
	vpment.e_type = type;
	vpment.e_dev = dev;
	vpment.e_word1 = word1;
	vpment.e_word2 = word2;
	trsave(0,0,&vpment,sizeof(vpment));
	splx(sps);
}
vpmsnap(type,dev,word1,word2)
char type,dev;
short word1,word2;
{
	static int vpmseqn;
	register sps;
	struct {
		short	s_seqn;
		char	s_type;
		char	s_dev;
		short	s_word1;
		short	s_word2;
		long	s_lbolt;
	} vpmsnt;

	sps = spl5();
	if(vpmseqn >= 077777)
		vpmseqn = 0;
	vpmsnt.s_seqn = ++vpmseqn;
	vpmsnt.s_type = type;
	vpmsnt.s_dev = dev;
	vpmsnt.s_word1 = word1;
	vpmsnt.s_word2 = word2;
	vpmsnt.s_lbolt = lbolt;
	trsave(1,0,&vpmsnt,sizeof(vpmsnt));
	splx(sps);
}
