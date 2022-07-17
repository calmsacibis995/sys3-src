/*
 *	UTS driver for DZ-11 with KMC assist
 */

#include "sys/param.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/file.h"
#include "sys/tty.h"
#include "sys/conf.h"
#include "sys/ioctl.h"
#include "sys/uba.h"
#include "sys/sysinfo.h"

struct device *dza_addr[];
int	dza_cnt;
struct tty dza_tty[];
short	dza_scan;
short	dza_stat;
short	dza_kmc[8];

char	dza_brk[8];	/* break bits - max 64 lines */
char	dza_speeds[16] = {
	0, 00, 01, 02, 03, 04, 0, 05,
	06, 07, 010, 012, 014, 016, 0, 0
	};

/*
 * Define op codes for commands to the KMC
 */
#define	XBUFIN	0
#define	IOCTL	1
#define	BASEIN	2
#define CMDIN	3
/*
 * Define sub command bits for CMDIN
 */
#define OFLUSH	(1<<1)	/* Flush output */
#define OSPND	(1<<2)	/* Suspend output */
#define ORSME	(1<<3)	/* Resume output */
#define SCHAR	(1<<4)	/* Send character in csr6 */

#define	BITS6	010
#define	BITS7	020
#define	TWOSB	040
#define	PENABLE	0100
#define	OPAR	0200
#define	RCVENB	010000

#define	IE	0140

struct device {
	short	dzacsr, dzarbuf;
	char	dzatcr, dzadtr;
	char	dzatbuf, dzabrk;
};
#define	dzalpr	dzarbuf
#define	dzamsr	dzabrk

#define	ON	1
#define	OFF	0


dzaopen(dev, flag)
{
	register struct tty *tp;
	register kf;
	extern dzaproc(), dzakint();

	if ((dev&077) >= dza_cnt) {
		u.u_error = ENXIO;
		return;
	}
	kf = 1<<(((dev>>6)&03)+8);
	if ((dza_stat&kf) == 0) {
		if (kmcset(dev,03,dzakint)) {
			u.u_error = ENXIO;
			return;
		}
		dza_stat |= kf;
	}
	tp = &dza_tty[dev&077];
	if ((tp->t_state&(ISOPEN|WOPEN)) == 0) {
		ttinit(tp);
		tp->t_proc = dzaproc;
		tp->t_state |= EXTPROC;
		dzaparam(dev);
	}
	spl5();
	if (tp->t_cflag&CLOCAL || dzamodem(dev, ON))
		tp->t_state |= CARR_ON;
	else
		tp->t_state &= ~CARR_ON;
	if (!(flag&FNDELAY))
	while ((tp->t_state&CARR_ON)==0) {
		tp->t_state |= WOPEN;
		sleep((caddr_t)&tp->t_canq, TTIPRI);
	}
	(*linesw[tp->t_line].l_open)(tp);
	spl0();
}

dzaclose(dev)
{
	register struct tty *tp;

	tp = &dza_tty[dev&077];
	(*linesw[tp->t_line].l_close)(tp);
	if (tp->t_cflag&HUPCL)
		dzamodem(dev, OFF);
}

dzaread(dev)
{
	register struct tty *tp;
	tp = &dza_tty[dev&077];
	(*linesw[tp->t_line].l_read)(tp);
}

dzawrite(dev)
{
	register struct tty *tp;
	tp = &dza_tty[dev&077];
	(*linesw[tp->t_line].l_write)(tp);
}

dzaioctl(dev, cmd, arg, mode)
register dev;
{
	switch(cmd) {
	default:
		if (ttiocom(&dza_tty[dev&077], cmd, arg, mode))
			dzaparam(dev);
	}
}

dzaparam(dev)
{
	register struct tty *tp;
	register struct device *dzaaddr;
	register flags, lpr;
	register struct cblock *cp;
	register i;

	tp = &dza_tty[dev&077];
	dzaaddr= dza_addr[(dev>>3)&07];
	dzaaddr->dzacsr = IE;
	if (dza_scan==0) {
		dzascan();
		dza_scan++;
	}
	flags = tp->t_cflag;
	if ((flags&CBAUD) == 0) {
		/* hang up line */
		dzamodem(dev, OFF);
		return;
	}
	lpr = (dza_speeds[flags&CBAUD]<<8)|(dev&07);
	if (flags&CREAD)
		lpr |= RCVENB;
	if (flags&CS6)
		lpr |= BITS6;
	if (flags&CS7)
		lpr |= BITS7;
	if (flags&PARENB) {
		lpr |= PENABLE;
		if (flags&PARODD)
			lpr |= OPAR;
	}
	if (flags&CSTOPB)
		lpr |= TWOSB;
	dzaaddr->dzalpr = lpr;
/*
 * Now pass the relevant parameters to the KMC
 */
	lpr = (dev>>3)&07;
	if ((dza_stat&(1<<lpr)) == 0) {
		dza_kmc[lpr] = dev&0370;
		kmcload(dza_kmc[lpr]|(dev&07),BASEIN,(int)dza_addr[lpr]-UBA_DEV,0);
		dza_stat |= 1<<lpr;
	}
	kmcload(dza_kmc[lpr]|(dev&07),IOCTL,tp->t_oflag,0);
}

dzarint(dev)
{
	register struct tty *tp;
	register c;
	register line;
	register struct device *dzaaddr;

	sysinfo.rcvint++;
	dzaaddr = dza_addr[dev];
	while ((c = dzaaddr->dzarbuf) < 0) {	/* char. present */
		line = (c>>8)&07;
		tp = &dza_tty[line|(dev<<3)];
		if (tp >= &dza_tty[dza_cnt])
			continue;
		if (!(tp->t_state&(ISOPEN|WOPEN)))
			continue;
		if (tp->t_cflag&CLOCAL || dzaaddr->dzamsr&(1<<line)) {
			if ((tp->t_state&CARR_ON) == 0) {
				wakeup(&tp->t_canq);
				tp->t_state |= CARR_ON;
			}
		} else {
			if (tp->t_state&CARR_ON) {
				signal(tp->t_pgrp, SIGHUP);
				dzaaddr->dzadtr &= ~(1<<line);
				tp->t_state &= ~CARR_ON;
				ttyflush(tp, (FREAD|FWRITE));
			}
			continue;
		}
		(*linesw[tp->t_line].l_input)(tp, c, 0);
	}
}

dzaxint(dev)
{
printf("dza: xint\n");
}

/*
 * Routine to handle interrupts from the KMC
 * This routine gets called from the KMC driver whenever
 * the KMC generates an unsolicited interrupt (VEC4 == 1)
 *
 * These interrupts are used by the KMC to notify dza.c
 * of events such as output buffer completions
 */
dzakint(dev, sel2, sel4, sel6)
{
	register struct tty *tp;
	register ttybit, bar;
	int dznr;

	sysinfo.xmtint++;
	ttybit = 1;
	bar = sel4&0377;
	dznr = dev&07;
	for (tp = &dza_tty[dznr<<3]; bar; tp++) {
		if (bar&ttybit) {
			bar &= ~ttybit;
			tp->t_state &= ~BUSY;
			dzaproc(tp, T_OUTPUT);
		}
		ttybit <<= 1;
	}
}

dzaproc(tp, cmd)
register struct tty *tp;
{
	extern ttrstrt();
	char *cp;
	int	unit;
	struct device *dzaaddr;
	register dev;
	register sel4, sel6;
	register n;

	dev = tp - dza_tty;
	dzaaddr = dza_addr[dev>>3];
	unit = 1<<(dev&07);
	sel4 = 0;
	switch(cmd) {

	case T_TIME:
		tp->t_state &= ~TIMEOUT;
		dza_brk[dev>>3] &= ~unit;
		dzaaddr->dzabrk = dza_brk[dev>>3];
		goto start;

	case T_WFLUSH:
		sel4 |= OFLUSH;

	case T_RESUME:
		tp->t_state &= ~TTSTOP;
		sel4 |= ORSME;
		goto start;

	case T_OUTPUT:
	start:
		if (tp->t_state&(TIMEOUT|TTSTOP|BUSY))
			break;
		if (tp->t_state&TTIOW && tp->t_outq.c_cc==0) {
			tp->t_state &= ~TTIOW;
			wakeup((caddr_t)&tp->t_oflag);
		}
		if (tp->t_buf!=NULL)
			putcf(tp->t_buf);
		if ((tp->t_buf = getcb(&tp->t_outq)) != NULL) {
			paddr_t addr;
			n = tp->t_buf->c_last - tp->t_buf->c_first;
			cp = &tp->t_buf->c_data[tp->t_buf->c_first];
			addr = ubmdata(cp);
			if (kmcload(dza_kmc[(dev>>3)&07]|(dev&07),
			    XBUFIN, loword(addr), (hiword(addr)<<14|(n&0377))) < 0)
				goto start;
			tp->t_state |= BUSY;
		}
		if (tp->t_state&OASLP && tp->t_outq.c_cc<=ttlowat[tp->t_cflag&CBAUD]) {
			tp->t_state &= ~OASLP;
			wakeup((caddr_t)&tp->t_outq);
		}
		break;

	case T_SUSPEND:
		tp->t_state |= TTSTOP;
		sel4 |= OSPND;
		break;

	case T_BLOCK:
		tp->t_state |= TBLOCK;
		sel4 |= SCHAR;
		sel6 = CSTOP;
		break;

	case T_RFLUSH:
		if (!(tp->t_state&TBLOCK))
			break;

	case T_UNBLOCK:
		tp->t_state &= ~TBLOCK;
		sel4 |= SCHAR;
		sel6 = CSTART;
		break;

	case T_BREAK:
		dza_brk[dev>>3] |= unit;
		dzaaddr->dzabrk = dza_brk[dev>>3];
		tp->t_state |= TIMEOUT;
		timeout(ttrstrt, tp, HZ/4);
		break;
	}
	if (sel4)
		kmcload(dza_kmc[(dev>>3)&07]|(dev&07), CMDIN, sel4, sel6);
}

dzamodem(dev, flag)
{
	register struct device *dzaaddr;
	register bit;

	dzaaddr = dza_addr[(dev>>3)&07];
	bit = 1<<(dev&07);
	if (flag==OFF)
		dzaaddr->dzadtr &= ~bit;
	else
		dzaaddr->dzadtr |= bit;
	return(dzaaddr->dzamsr & bit);
}

dzascan()
{
	register i;
	register struct device *dzaaddr;
	register struct tty *tp;
	char	bit;

	for (i=0; i<dza_cnt; i++) {
		tp = &dza_tty[i];
		if (!(tp->t_state&(ISOPEN|WOPEN)))
			continue;
		dzaaddr = dza_addr[i>>3];
		bit = 1<<(i&07);
		if (tp->t_cflag&CLOCAL || dzaaddr->dzamsr&bit) {
			if ((tp->t_state&CARR_ON)==0) {
				wakeup(&tp->t_canq);
				tp->t_state |= CARR_ON;
			}
		} else {
			if (tp->t_state&CARR_ON) {
				if (tp->t_state&ISOPEN) {
					signal(tp->t_pgrp, SIGHUP);
					dzaaddr->dzadtr &= ~bit;
					ttyflush(tp, (FREAD|FWRITE));
				}
				tp->t_state &= ~CARR_ON;
			}
		}
	}
	timeout(dzascan, 0, 120);
}

dzaclr()
{
	register dev;
	register struct tty *tp;

	if (dza_stat == 0)
		return;
	for (dev = 0; dev < 4; dev++)
		if (dza_stat&(1<<(dev+8)))
			kmcdclr(dev);
	dza_stat = 0;
	for (dev = 0; dev < dza_cnt; dev++) {
		tp = &dza_tty[dev];
		if ((tp->t_state&(ISOPEN|WOPEN)) == 0)
			continue;
		dzaparam(dza_kmc[(dev>>3)&07]|(dev&07));
		dzamodem(dev, ON);
		tp->t_state &= ~BUSY;
		dzaproc(tp, T_OUTPUT);
	}
}
