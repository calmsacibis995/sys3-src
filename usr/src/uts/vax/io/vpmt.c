/*Version 2*/
#include "sys/param.h"
#include "sys/tty.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/file.h"
#include "sys/vpm.h"
#include "sys/vpmd.h"
#include "sys/ioctl.h"

#define BISYNC	6		/*BISYNC interpreter*/
#define VPMTS	4		/*Number of Virtual Protocol Machines*/
#define XBQMAX	2		/*Max xmit buffers in xmit queue*/
#define BUFDES	(XBQMAX + 2)*VPMTS		/*Number of buffer descriptors*/
#define BUFSIZE	512		/*Buffer size*/
#define NOARG	0		/*Null argument*/
#define VPMPRI	(PZERO+5)

/*vpm state bits*/

#define OPEN	01
#define WCLOSE	02
#define VPMERR	04
#define XOPEN	010

int vpm_cnt;

/*Buffer discriptors*/

struct vpmbd vpmbd[BUFDES];

/*Per vmp structure*/

struct vpmt {
	struct clist vt_rcvq;
	struct vpmbd *vt_rbc;
	short	vt_state;	/*Status info*/
} vpmt[VPMTS];
extern struct vpmbd *vpmdeq();
extern struct buf *getablk();
extern struct buf *geteblk();

vpmopen(dev, flag)
{
	register struct vpmt *vp;
	register int ec;
	extern struct vpmbd *vpmbget();
	extern vpmtrint();

	if (TDEV(dev) >= vpm_cnt) {
		u.u_error = ENXIO;
		return;
	}
	if (((vp = &vpmt[TDEV(dev)])->vt_state&OPEN) == 0) {
		if (ec = vpmstart(dev, BISYNC, vpmtrint)) {
			vpmsave('p', dev, ec, 0);
			u.u_error = ec;
			return;
		}
		if ((flag&FREAD) && (flag&FWRITE))
			vp->vt_state |= (XOPEN|OPEN);
		else
			vp->vt_state |= OPEN;
		vpmemptq(dev, vpmbget(dev));
		vpmemptq(dev, vpmbget(dev));
	} else {
		if ((vp->vt_state&XOPEN) || ((flag&FREAD) && (flag&FWRITE))) {
			u.u_error = EBUSY;
			return;
		}
	}
	vpmsave('o', dev, vp->vt_state, 0);
}
vpmclose(dev)
{
	struct vpmt *vp;
	struct vpmbd *bdp;

	vp = &vpmt[TDEV(dev)];
	vp->vt_state |= WCLOSE;
	vpmstop(dev);
	while(vpmdeq(&vp->vt_rcvq));
	for(bdp = &vpmbd[0];bdp < &vpmbd[BUFDES];bdp++)
		if (bdp->d_vpmtdev == TDEV(dev))
			vpmbrtn(bdp);
	vpmsave('c', dev, vp->vt_state, 0);
	vp->vt_state = 0;
	vp->vt_rbc = 0;
	if (vp->vt_state&VPMERR)
		u.u_error = EIO;
}
vpmwrite(dev)
{
	register struct vpmt *vp;
	register struct vpmbd *dp;
	register ct;

	vp = &vpmt[TDEV(dev)];
	if (vp->vt_state&VPMERR) {
		u.u_error = EIO;
		return;
	}
	while(u.u_count) {
		spl5();
		while(vpmxmtq(dev, 0) >= XBQMAX && (vp->vt_state&VPMERR) == 0)
			sleep((caddr_t)&vp->vt_state, VPMPRI);
		spl0();
		if (vp->vt_state&VPMERR) {
			u.u_error = EIO;
			return;
		}
		dp = vpmbget(dev);
		ct = min(u.u_count, BUFSIZE);
		MOVE(paddr(dp->d_buf), ct, B_WRITE);
		dp->d_ct = ct;
		vpmsave('w', dev, ct, dp);
		vpmxmtq(dev, dp);
	}
}
vpmread(dev)
{
	register struct vpmt *vp;
	register struct vpmbd *dp;
	register cnt;

	vp = &vpmt[TDEV(dev)];
	if (vp->vt_state&VPMERR) {
		u.u_error = EIO;
		return;
	}
	if (vp->vt_rbc == 0) {
		spl5();
		while((vp->vt_rbc = vpmdeq(&vp->vt_rcvq)) == 0 && (vp->vt_state&VPMERR) == 0)
			sleep((caddr_t)&vp->vt_rcvq, VPMPRI);
		spl0();
		if (vp->vt_state&VPMERR) {
			u.u_error = EIO;
			return;
		}
	}
	dp = vp->vt_rbc;
	cnt = dp->d_ct - dp->d_bos;
	cnt = min(cnt, u.u_count);
	MOVE(paddr(dp->d_buf) + dp->d_bos, cnt, B_READ);
	vpmsave('r', dev, cnt, dp->d_bos);
	if ((dp->d_bos += cnt) == (dp->d_ct)) {
		vp->vt_rbc = 0;
		dp->d_ct = BUFSIZE;
		dp->d_bos = 0;
		vpmemptq(dev, dp);
	}
}
vpmioctl(dev, cmd, arg, mode)
{
	register struct vpmt *vp;
	char cmdar[4], *cp;
	extern char *vpmrpt(), *vpmerrs();

	vp = &vpmt[TDEV(dev)];
	switch(cmd) {
	case VPMCMD:
		copyin(arg, cmdar, sizeof(cmdar));
		vpmcmd(dev, cmdar);
		return;
	case VPMERRS:
		cp = vpmerrs(dev, 8);
		copyout(cp, arg, 8);
		return;
	case VPMRPT:
		if (cp = vpmrpt(dev)) {
			copyout(cp, arg, 4);
			u.u_rval1 = 1;
		}
		return;
	default:
		u.u_error = EINVAL;
		return;
	}
}
vpmtrint(dev, type, bdp)
{
	register struct vpmt *vp;

	vp = &vpmt[TDEV(dev)];
	switch(type) {
		case RRTNXBUF:
			vpmbrtn(bdp);
			wakeup((caddr_t)&vp->vt_state);
			break;
		case RRTNRBUF:
			vpmenq(bdp, &vp->vt_rcvq);
			wakeup((caddr_t)&vp->vt_rcvq);
			break;
		case RRTNEBUF:
			vpmbrtn(bdp);
			break;
		case ERRTERM:
			vp->vt_state |= VPMERR;
			wakeup((caddr_t)&vp->vt_rcvq);
			wakeup((caddr_t)&vp->vt_state);
			break;
	}
}
struct vpmbd *
vpmbget(dev)
{

	register struct vpmbd *dp;
	paddr_t	addr;

	for(dp = &vpmbd[0];dp < &vpmbd[BUFDES];dp++)
		if (dp->d_buf == 0) {
			dp->d_buf = (struct buf *)1;
			break;
		}
	if (dp < &vpmbd[BUFDES]) {
		dp->d_buf = GETBLK;
		addr = ubmaddr(dp->d_buf, 0);
		dp->d_adres = loword(addr);
		dp->d_hbits = hiword(addr);
		dp->d_bos = 0;
		dp->d_ct = BUFSIZE;
		dp->d_vpmtdev = TDEV(dev);
		return(dp);
	}
	return(0);
}
vpmbrtn(dp)
struct vpmbd *dp;
{

	if (dp->d_buf != NULL) {
		brelse(dp->d_buf);
		dp->d_buf = NULL;
	}
}
