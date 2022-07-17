/*
 * KMC11 microprocessor driver
 */

#include "sys/param.h"
#include "sys/tty.h"
#include "sys/ioctl.h"
#include "sys/buf.h"
#include "sys/dir.h"
#include "sys/user.h"

struct device *kmc_addr[];
int	kmc_cnt;

#define	MAXKMC	4
struct kmc {
	struct clist k_inq;
	char	k_stat;
	char	k_type;
	int	(*k_rint)();
} kmc [MAXKMC];

#define	KMC11A	1
#define	KMC11B	2
#define	KASIZE	1024
#define	KBSIZE	4096

#define	KSTEP	1
#define	KMS	2
#define	KCSR	3
#define	KSTOP	4
#define	KMCLR	5
#define	KRUN	6
#define	KLU	7

#define	RUN	(1<<7)
#define	MCLR	(1<<6)
#define	CWRT	(1<<5)
#define	LUB	(1<<4)
#define	LUA	(1<<3)
#define	ROMO	(1<<2)
#define	ROMI	(1<<1)
#define	STEP	(1<<0)

#define RDYO	0200
#define RDYI	020
#define RQI	0200
#define IEI	01
#define IEO	020

#define	STYPE	017
#define SRUN	020
#define SRINT	040
#define	SOPEN	0100

struct device {
union {
	char	b[8];
	ushort	w[4];
} un;
};

#define	bsel0	un.b[0]
#define	bsel1	un.b[1]
#define	bsel2	un.b[2]
#define	bsel3	un.b[3]
#define	bsel4	un.b[4]
#define	bsel5	un.b[5]
#define	bsel6	un.b[6]
#define	bsel7	un.b[7]
#define	sel0	un.w[0]
#define	sel2	un.w[1]
#define	sel4	un.w[2]
#define	sel6	un.w[3]

kmcopen(dev, flag)
{
	register struct device *kp;
	register struct kmc *tp;
	register sav;

	if (dev>=kmc_cnt || (tp = &kmc[dev])->k_stat&SOPEN) {
		u.u_error = ENXIO;
		return;
	}
	tp->k_stat |= SOPEN;
	if (tp->k_type==0) {
		kp = kmc_addr[dev];
		kp->bsel1 = ROMO;
		kp->sel4 = 0;
		sav = kp->sel6;
		kp->sel6 = ~sav;
		if (kp->sel6 != sav) {
			tp->k_type = KMC11B;
			kp->sel6 = sav;
		} else
			tp->k_type = KMC11A;
		kp->bsel1 = 0;
	}
}

kmcclose(dev)
{
	kmc[dev].k_stat &= ~SOPEN;
}

kmcread(dev)
{
	register struct device *kp;
	register ad;
	int	dsize;
	ushort	sav;

	if (kmc[dev].k_stat&SRUN)
		return;
	dsize = (kmc[dev].k_type==KMC11A)?KASIZE:KBSIZE;
	kp = kmc_addr[dev];
	kp->bsel1 = 0;
	do {
		ad = u.u_offset;
		if (ad<dsize*2) {
			if (ad&1) {
				u.u_error = ENXIO;
				break;
			}
			ad >>= 1;
			kp->bsel1 = ROMO;
			kp->sel4 = ad;
			passc(kp->bsel6);
			passc(kp->bsel7);
			kp->bsel1 = 0;
		} else if (ad -= dsize*2, ad<dsize) {
			kp->bsel1 = ROMO;
			kp->sel4 = 0;
			sav = kp->sel6;
			kp->bsel1 = ROMI;
			kp->sel6 = 010000|(ad&0377);	/* mov ad,mar */
			kp->bsel1 = ROMI|STEP;
			kp->bsel1 = ROMI;
			kp->sel6 = 04000|((ad>>8)&0377);	/* mov %ad,%mar */
			kp->bsel1 = ROMI|STEP;
			kp->bsel1 = ROMI;
			kp->sel6 = 055222;	/* mov mem,csr2|mar++ */
			kp->bsel1 = ROMI|STEP;
			passc(kp->bsel2);
			kp->bsel1 = ROMI;
			kp->sel6 = sav;
			kp->bsel1 = 0;
		} else
			break;
	} while (u.u_error==0 && u.u_count);
}

kmcwrite(dev)
{
	register struct device *kp;
	register ad;
	int	dsize;
	short	ins;
	ushort	sav;

	if (kmc[dev].k_stat&SRUN)
		return;
	dsize = (kmc[dev].k_type==KMC11A)?KASIZE:KBSIZE;
	kp = kmc_addr[dev];
	kp->bsel1 = 0;
	while (u.u_error==0 && u.u_count) {
		ad = u.u_offset;
		if (ad<dsize*2) {
			if (ad&1) {
				u.u_error = ENXIO;
				break;
			}
			kp->bsel1 = ROMO;
			kp->sel4 = ad>>1;
			lobyte(ins) = cpass();
			hibyte(ins) = cpass();
			kp->sel6 = ins;
			kp->bsel1 |= CWRT;
			kp->bsel1 = 0;
		} else if (ad -= dsize*2, ad<dsize) {
			kp->bsel1 = ROMO;
			kp->sel4 = 0;
			sav = kp->sel6;
			kp->bsel1 = ROMI;
			kp->sel6 = 010000|(ad&0377);	/* mov ad,mar */
			kp->bsel1 = ROMI|STEP;
			kp->bsel1 = ROMI;
			kp->sel6 = 04000|((ad>>8)&0377);	/* mov %ad,%mar */
			kp->bsel1 = ROMI|STEP;
			kp->bsel1 = ROMI;
			kp->bsel2 = cpass();
			kp->sel6 = 0136440;	/* mov csr2,mem|mar++ */
			kp->bsel1 = ROMI|STEP;
			kp->bsel1 = ROMI;
			kp->sel6 = sav;
			kp->bsel1 = 0;
		} else
			break;
	}
}

kmcioctl(dev, cmd, arg, mode)
{
	register struct device *kp;
	register cd;
	register struct kmc *tp;
	int	kk[3];
	ushort	sav;

	if (cmd != TIOCSETP)
		return;
	if (copyin(arg, kk, sizeof(kk))) {
		u.u_error = EFAULT;
		return;
	}
	kp = kmc_addr[dev];
	tp = &kmc[dev];
	cd = kk[0];
	if (tp->k_stat&SRUN && cd!=KSTOP && cd!=KMCLR) {
		u.u_error = EIO;
		return;
	}
	switch (cd) {
	case KMCLR:
		spl7();
		kp->bsel1 = MCLR;
		spl0();
	case KSTOP:
		tp->k_stat &= ~SRUN;
		kp->bsel1 = 0;
		break;
	case KMS:
		kp->bsel1 = ROMI|ROMO;
		sav = kp->sel6;
		kp->bsel1 = ROMI;
		kp->sel6 = kk[2];
		kp->bsel1 = ROMI|STEP;
		kp->bsel1 = ROMI;
		kp->sel6 = sav;
		goto lcsr;
	case KSTEP:
		kp->bsel1 |= STEP;
	case KCSR:
	lcsr:
		suword((caddr_t)kk[1], kp->sel0);
		suword((caddr_t)kk[1]+2, kp->sel2);
		suword((caddr_t)kk[1]+4, kp->sel4);
		suword((caddr_t)kk[1]+6, kp->sel6);
		kp->bsel1 = 0;
		break;
	case KRUN:
		tp->k_stat &= ~STYPE;
		tp->k_stat |= (kk[2]&STYPE)|SRUN;
		kp->bsel1 |= RUN;
		if (tp->k_stat&SRINT) {
			spl5();
			kmcrint(dev);
			spl0();
		}
		break;
	case KLU:
		kp->bsel1 = kk[2]&(LUA|LUB);
		break;
	}
}

kmcrint(dev)
{
	register struct device *kp;
	register struct kmc *tp;

	kp = kmc_addr[dev];
	tp = &kmc[dev];
	kp->sel0 &= ~IEI;
	while (kp->sel2&RDYI) {
		kp->bsel2 = getc(&tp->k_inq)|RDYI;
		kp->bsel3 = getc(&tp->k_inq);
		kp->bsel4 = getc(&tp->k_inq);
		kp->bsel5 = getc(&tp->k_inq);
		kp->bsel6 = getc(&tp->k_inq);
		kp->bsel7 = getc(&tp->k_inq);
		if (tp->k_inq.c_cc==0) {
			kp->sel0 &= ~RQI;
			kp->sel2 &= ~RDYI;
			return;
		}
		kp->sel2 &= ~RDYI;
	}
	if (tp->k_inq.c_cc)
		kp->sel0 |= IEI|RQI;
}

kmcxint(dev)
{
	register struct device *kp;
	register struct kmc *tp;
	int p1, p2, p3, p4;

	kp = kmc_addr[dev];
	tp = &kmc[dev];
	kp->sel0 &= ~IEO;
	while(kp->sel2&RDYO) {
		p1 = (dev<<6)|(kp->bsel3&077);
		p2 = kp->bsel2&017;
		p3 = kp->sel4;
		p4 = kp->sel6;
		kp->sel2 &= ~RDYO;
		if (tp->k_stat&SRINT)
			(*tp->k_rint)(p1, p2, p3, p4);
	}
	kp->sel0 |= IEO;
}

kmcload(dev, p1, p2, p3)
{
	register struct device *kp;
	register struct kmc *tp;
	register unit;
	int	sps;

	unit = (dev>>6)&03;
	tp = &kmc[unit];
	if (!(tp->k_stat&SRUN))
		return(-1);
	kp = kmc_addr[unit];
	sps = spl5();
	kp->sel0 |= RQI;
	putc(lobyte(p1), &tp->k_inq);
	putc(dev&077, &tp->k_inq);
	putc(lobyte(p2), &tp->k_inq);
	putc(hibyte(p2), &tp->k_inq);
	putc(lobyte(p3), &tp->k_inq);
	putc(hibyte(p3), &tp->k_inq);
	kmcrint(unit);
	splx(sps);
	return(tp->k_inq.c_cc);
}

kmcset(dev, type, rint)
int (*rint)();
{
	register struct device *kp;
	register struct kmc *tp;
	register unit;

	unit = (dev>>6)&03;
	kp = kmc_addr[unit];
	tp = &kmc[unit];
	if ((tp->k_stat&(STYPE|SRUN|SOPEN))!=((type&STYPE)|SRUN))
		return (1);
	tp->k_stat |= SRINT;
	tp->k_rint = rint;
	kp->sel0 |= IEO;
	return(0);
}

kmcdclr(dev)
register dev;
{
	register struct kmc *tp;
	register struct device *kp;

	if (dev < 0 || dev >= kmc_cnt)
		return;
	tp = &kmc[dev];
	while (getc(&tp->k_inq) >= 0) ;
	kp = kmc_addr[dev];
	kp->sel0 = 0;
	kp->sel2 = 0;
}
