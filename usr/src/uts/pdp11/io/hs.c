/*
 * RS03/04 disk driver
 */

#include "sys/param.h"
#include "sys/buf.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/systm.h"
#include "sys/elog.h"
#include "sys/iobuf.h"

struct device {
	int	hscs1, hswc, hsba, hsda;
	int	hscs2, hsds, hser, hsas;
	int	hsla, hsdb, hsmr, hsdt;
	int	hsbae, hscs3;
};

struct	iostat	hsstat[8];
struct	iobuf	hstab =	tabinit(HS0,hsstat);
struct	buf	rhsbuf;

struct device *hs_addr;
int	hs_cnt;

#define TRE	040000	/* hscs1 - transfer error */

#define GO	01
#define RCLR	010
#define	WCOM	060
#define	RCOM	070
#define	IE	0100

#define	DRY	0200	/* hsds - Drive Ready */
#define	MOL	010000	/* hsds - Medium OnLine */
#define	CTLCLR	040	/* hscs2 - CLEAR */

#define	wtime	io_s2
#define	WOK	0
#define	WABORT	1
#define	WMOL	4
#define	WERR	30
#define	HSTIMEOUT	15*HZ

#define rh70	(cputype == 70)

hsopen(dev)
{
	hstab.io_addr = (physadr)hs_addr;
	hstab.io_nreg = rh70?NDEVREG:NDEVREG-2;
}

hsclose(dev)
{
}

hsstrategy(bp)
register struct buf *bp;
{
	daddr_t	mblks;

	if (bp->b_dev&010)
		mblks = 2048; /* RS04 */
	else	mblks = 1024; /* RS03 */
	if (bp->b_blkno >= mblks) {
		if (bp->b_blkno == mblks && bp->b_flags&B_READ)
			bp->b_resid = bp->b_bcount;
		else {
			bp->b_flags |= B_ERROR;
			bp->b_error = ENXIO;
		}
		iodone(bp);
		return;
	}
	bp->av_forw = 0;
	spl5();
	if (hstab.b_actf==0)
		hstab.b_actf = bp; else
		hstab.b_actl->av_forw = bp;
	hstab.b_actl = bp;
	if (hstab.b_active==0)
		hsstart();
	spl0();
}

hsstart()
{
	register struct buf *bp;
	register struct device *rp;
	register addr;
	int unit;
	extern hstimer();

	if ((bp = hstab.b_actf) == 0)
		return;
	hstab.b_active++;
	rp = hs_addr;
	addr = bp->b_blkno;
	addr <<= 1;
	if ((bp->b_dev&010) == 0)
		addr <<= 1;
	unit = minor(bp->b_dev) & 07;
	rp->hscs2 = unit;
	if ((rp->hsds&MOL) == 0) {
		if (hstab.wtime)
			return;
		if (!pwr_act)
			hstab.wtime = WMOL;
		else
			hstab.wtime = WERR;
		if ((hstab.b_flags&B_TIME) == 0) {
			hstab.b_flags |= B_TIME;
			timeout(hstimer, 0, HSTIMEOUT);
		}
		return;
	}
	hstab.wtime = WOK;
	hsstat[unit].io_ops++;
	if (rh70)
		rp->hsbae = hiword(bp->b_paddr);
	rp->hsda = addr;
	rp->hsba = loword(bp->b_paddr);
	rp->hswc = -(bp->b_bcount>>1);
	addr = ((hiword(bp->b_paddr)&3)<<8)|IE|GO;
	if (bp->b_flags & B_READ)
		addr |= RCOM;
	else
		addr |= WCOM;
	blkacty |= (1<<HS0);
	if (pwr_act >= 0)
		rp->hscs1 = addr;
}

hsintr()
{
	register struct buf *bp;
	register struct device *rp;

	rp = hs_addr;
	if (hstab.b_active == 0) {
		logstray(rp);
		return;
	}
	blkacty &= ~(1<<HS0);
	bp = hstab.b_actf;
	hstab.b_active = 0;
	if (rp->hscs1 & TRE) {	/* error bit */
		hstab.io_stp = &hsstat[minor(bp->b_dev)&07];
		fmtberr(&hstab,0);
		if (hstab.b_errcnt > 2)
			deverr(&hstab, rp->hscs2, 0);
		rp->hscs1 = RCLR|GO;
		if (++hstab.b_errcnt <= 10) {
			hsstart();
			return;
		}
		bp->b_flags |= B_ERROR;
	}
	if (hstab.io_erec)
		logberr(&hstab,bp->b_flags&B_ERROR);
	hstab.b_errcnt = 0;
	hstab.b_actf = bp->av_forw;
	bp->b_resid = (-rp->hswc)<<1;
	iodone(bp);
	hsstart();
}

hsread(dev)
{
	if (physck((daddr_t)((dev&010) ? 2048 : 1024), B_READ))
		physio(hsstrategy, &rhsbuf, dev, B_READ);
}

hswrite(dev)
{
	if (physck((daddr_t)((dev&010) ? 2048 : 1024), B_WRITE))
		physio(hsstrategy, &rhsbuf, dev, B_WRITE);
}

hstimer()
{
	register struct buf *bp;

	hstab.b_flags &= ~B_TIME;
	if (hstab.wtime == WOK)
		return;
	hsstart();
	if (hstab.wtime == WOK)
		return;
	if (hstab.wtime == WABORT) {
		if (hstab.io_erec)
			logberr(&hstab, B_ERROR);
		while (bp = hstab.b_actf) {
			bp->b_flags |= B_ERROR;
			hstab.b_actf = bp->av_forw;
			iodone(bp);
		}
		hstab.b_active = 0;
		hstab.b_errcnt = 0;
		printf("RS03/4 not available\n");
	}
	hstab.wtime--;
	if (hstab.wtime == WOK)
		return;
	hstab.b_flags |= B_TIME;
	timeout(hstimer, 0, HSTIMEOUT);
}

hsclr()
{
	register struct device *rp;

	rp = hs_addr;
	rp->hscs2 = CTLCLR;
	hstab.b_active = 0;
	blkacty &= ~(1<<HS0);
	hsstart();
}
