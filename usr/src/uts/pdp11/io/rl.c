/*
 *  RL disk driver
 */

#include "sys/param.h"
#include "sys/buf.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/systm.h"
#include "sys/elog.h"
#include "sys/iobuf.h"

#define NBLK1 10240
#define NBLK2 20480
#define RL02 0200	/* bit 7 indicates an rl02 present if set */
#define RLCYLSZ 10240
#define RLSECSZ 256

#define RESET 013
#define STAT 03
#define GETSTAT 04
#define WCOM 012
#define RCOM 014
#define SEEK 06
#define SEEKHI 5
#define SEEKLO 1
#define RDHDR 010
#define IENABLE 0100
#define CRDY 0200
#define OPI 02000
#define CRCERR 04000
#define TIMOUT 010000
#define NXM 020000
#define DE  040000

struct device
{
	short rlcs, rlba, rlda, rlmp;
};

struct	device	*rl_addr[];
short	rl_cnt;

struct 
{
	short	cn[8];		/* location of heads for each drive */
	short	dn;		/* drive number */
	short	com;		/* read or write command word */
	short	chn;		/* cylinder and head number */
	unsigned bleft;		/* bytes left to be transferred */
	unsigned bpart;		/* number of bytes transferred */
	short	sn;		/* sector number */
	paddr_t	addr;		/* address of memory for transfer */

}	rl = {-1,-1,-1,-1,-1,-1,-1,-1};

int nblk[4] = {-1, -1, -1, -1} ;
struct	iostat	rlstat[4];
struct	iobuf	rltab =	tabinit(RL0,rlstat);
struct	buf	rrlbuf;

rlopen(dev)
{
	struct device *dp;
	int dnum = minor(dev)&03;

	if(dev>=rl_cnt) {
		u.u_error = ENXIO;
		return;
	}
	spl5();
	dp = rl_addr[dev>>2];
	if( nblk[dnum] == -1) {
		dp->rlda = STAT;
		dp->rlcs = (dnum<<8) | GETSTAT ;
		while ( (dp->rlcs & CRDY) == 0)
			;
		nblk[dnum]=(dp->rlmp & RL02) ? NBLK2 : NBLK1 ;
	}
	spl0();
	rltab.io_addr = (physadr)rl_addr[dev>>2];
	rltab.io_nreg = NDEVREG;
	if ((rltab.b_flags&B_ONCE)==0) {
		rltab.b_flags |= B_ONCE;
		rltab.io_s1 = ubmalloc(0-1);
	}
}

rlclose(dev)
{
}

rlstrategy(bp)
register struct buf *bp;
{
	if(bp->b_blkno >= nblk[minor(bp->b_dev)&03]) {
		if(bp->b_blkno == nblk[minor(bp->b_dev)&03] && bp->b_flags&B_READ)
			bp->b_resid = bp->b_bcount;
		else {
			bp->b_flags |= B_ERROR;
			bp->b_error = ENXIO;
		}
		iodone(bp);
		return;
	}
	bp->av_forw = NULL;
	spl5();
	if(rltab.b_actf == NULL)
		rltab.b_actf = bp;
	else
		rltab.b_actl->av_forw = bp;
	rltab.b_actl = bp;
	if(rltab.b_active == 0)
		rlstart();
	spl0();
}

rlstart()
{

	register struct device *rp;
	register struct buf *bp;

	if ((bp = rltab.b_actf) == NULL)
		return;
	rltab.b_active++;
	rl.dn = minor(bp->b_dev)&3;
	rl.chn = bp->b_blkno/20;
	rl.sn = (bp->b_blkno%20) << 1;
	rl.bleft = bp->b_bcount;
	rl.addr = ubmaddr(bp,rltab.io_s1);
	rl.com = (rl.dn << 8) | IENABLE;
	if (bp->b_flags & B_READ)
		rl.com |= RCOM;
	else
		rl.com |= WCOM;
	rp = rl_addr[minor(bp->b_dev)>>2];
	rlio(rp);
}

rlintr()
{

	register struct buf *bp;
	register struct device *rp;
	register short status;

	bp = rltab.b_actf;
	rp = rl_addr[minor(bp->b_dev)>>2];
	if (rltab.b_active == 0) {
		logstray(rp);
		return;
	}
	blkacty &= ~(1<<RL0);
	bp = rltab.b_actf;
	if (rp->rlcs < 0) {		/* error bit */
		if (rp->rlcs & 036000) {
			rltab.io_stp = &rlstat[minor(bp->b_dev)];
			fmtberr(&rltab,0);
			if(rltab.b_errcnt > 2)
				deverr(&rltab, rp->rlcs, rp->rlda);
		}
		if (rp->rlcs & 040000) {
			rp->rlda = STAT;
			rlstat[minor(bp->b_dev)].io_misc++;
			rp->rlcs = (rl.dn << 8) | GETSTAT;
			while ((rp->rlcs & CRDY) == 0)
				;
			status = rp->rlmp;
			if(!status&01000 || rl.cn[rl.dn] != -1) {
				rltab.io_stp = &rlstat[minor(bp->b_dev)];
				fmtberr(&rltab,0);
				if(rltab.b_errcnt > 2)
					deverr(&rltab, status, rp->rlda);
			}
			rp->rlda = RESET;
			rp->rlcs = (rl.dn << 8) | GETSTAT;
			while ((rp->rlcs & CRDY) == 0)
				;
			if(status & 01000) {
				rlstart();
				return;
			}
		}
		if (++rltab.b_errcnt <= 10) {
			rl.cn[rl.dn] = -1;
			rlstart();
			return;
		}
		bp->b_flags |= B_ERROR;
		rl.bpart = rl.bleft;
	}

	if ((rl.bleft -= rl.bpart) > 0) {
		rl.addr += rl.bpart;
		rl.sn=0;
		rl.chn++;
		rlio(rp);
		return;
	}
	if(rltab.io_erec)
		logberr(&rltab,bp->b_flags&B_ERROR);
	rltab.b_active = 0;
	rltab.b_errcnt = 0;
	rltab.b_actf = bp->av_forw;
	bp->b_resid = 0;
	iodone(bp);
	rlstart();
}

rlio(rp)
register struct device *rp;
{

	register dif,head;
	
	if (rl.cn[rl.dn] < 0) {
		rlstat[rl.dn].io_misc++;
		blkacty |= (1<<RL0);
		rp->rlcs = (rl.dn << 8) | RDHDR;
		while ((rp->rlcs&CRDY) == 0)
			;
		blkacty &= ~(1<<RL0);
		rl.cn[rl.dn] = (rp->rlmp&077700) >> 6;
	}
	dif =(rl.cn[rl.dn] >> 1) - (rl.chn >>1);
	head = (rl.chn & 1) << 4;
	if (dif < 0)
		rp->rlda = (-dif <<7) | SEEKHI | head;
	else
		rp->rlda = (dif << 7) | SEEKLO | head;
	rlstat[rl.dn].io_misc++;
	blkacty |= (1<<RL0);
	rp->rlcs = (rl.dn << 8) | SEEK;
	while ((rp->rlcs&CRDY) == 0)
		;
	blkacty &= ~(1<<RL0);
	rl.cn[rl.dn] = rl.chn;
	if (rl.bleft < (rl.bpart = RLCYLSZ - (rl.sn * RLSECSZ)))
		rl.bpart = rl.bleft;
	rp->rlda = (rl.chn << 6) | rl.sn;
	rp->rlba = loword(rl.addr);
	rp->rlmp = -(rl.bpart >> 1);
	rlstat[rl.dn].io_ops++;
	blkacty |= (1<<RL0);
	rp->rlcs = rl.com | hiword(rl.addr) << 4;
}

rlread(dev)
{

	if (physck((daddr_t) nblk[minor(dev)&03], B_READ))
		physio(rlstrategy, &rrlbuf, dev, B_READ);
}

rlwrite(dev)
{

	if (physck((daddr_t)nblk[minor(dev)&03],B_WRITE))
		physio(rlstrategy, &rrlbuf, dev, B_WRITE);
}

rlclr()
{
	rltab.b_active = 0;
	rlstart();
}
