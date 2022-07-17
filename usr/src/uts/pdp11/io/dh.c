/*
 *	DH11 & DM11-BB driver for multiple units
 */

#include "sys/param.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/file.h"
#include "sys/tty.h"
#include "sys/conf.h"
#include "sys/sysinfo.h"

struct device *dh_addr[];
int	dh_cnt;
struct tty dh_tty[];
struct inter dm_inter[];
short	dhsar[8];
static char dh_xon = CSTART;
static char dh_xoff = CSTOP;

#define	BITS6	01
#define	BITS7	02
#define	TWOSB	04
#define	PENABLE	020
#define	OPAR	040

#define	IENAB	030100
#define	XINT	0100000
#define	CLRNEM	0400

struct device {
	short dhcsr, dhnxch, dhlpr, dhcar;
	short dhbcr, dhbar, dhbrk, dhsilo;
};

struct devdm *dm_addr[];

#define	DONE	0200
#define	SCENABL	040
#define	BSY	020
#define	TURNON	07	/* RTS, DTR, line enable */
#define	TURNOFF	1	/* line enable only */
#define	CARRIER	0100

struct devdm {
	short	dmcsr, dmlstat;
};

dhopen(dev, flag)
{
	register struct tty *tp;
	extern dhproc();

	if (dev >= dh_cnt) {
		u.u_error = ENXIO;
		return;
	}
	tp = &dh_tty[dev];
	if ((tp->t_state&(ISOPEN|WOPEN)) == 0) {
		ttinit(tp);
		tp->t_proc = dhproc;
		dhparam(dev);
	}
	spl5();
	if (tp->t_cflag&CLOCAL || dmctl(dev, TURNON)&CARRIER)
		tp->t_state |= CARR_ON;
	if (!(flag&FNDELAY))
	while ((tp->t_state&CARR_ON)==0) {
		tp->t_state |= WOPEN;
		sleep(&tp->t_canq, TTIPRI);
	}
	(*linesw[tp->t_line].l_open)(tp);
	spl0();
}

dhclose(dev)
{
	register struct tty *tp;

	tp = &dh_tty[dev];
	(*linesw[tp->t_line].l_close)(tp);
	if (tp->t_cflag&HUPCL) {
		spl5();
		dmctl(dev, TURNOFF);
		spl0();
	}
}

dhread(dev)
{
	register struct tty *tp;

	tp = &dh_tty[dev];
	(*linesw[tp->t_line].l_read)(tp);
}

dhwrite(dev)
{
	register struct tty *tp;

	tp = &dh_tty[dev];
	(*linesw[tp->t_line].l_write)(tp);
}

dhioctl(dev, cmd, arg, mode)
register dev;
{
	switch(cmd) {
	default:
		if (ttiocom(&dh_tty[dev], cmd, arg, mode))
			dhparam(dev);
	}
}

dhparam(dev)
{
	register struct tty *tp;
	register flags, lpr;
	register struct device *rp;

	tp = &dh_tty[dev];
	rp = dh_addr[dev>>4];
	flags = tp->t_cflag;
	lpr = (flags&CBAUD)<<10;
	if (flags&CREAD)
		lpr |= (flags&CBAUD)<<6;
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
	spl5();
	rp->dhcsr &= ~017;
	rp->dhcsr |= (dev&017) | IENAB;
	if ((flags&CBAUD) == 0) {
		/* Hang up line */
		if (tp->t_state&CARR_ON) {
			rp->dhbcr = -1;
			dmctl(dev, TURNOFF);
		}
		spl0();
		return;
	}
	rp->dhlpr = lpr;
	spl0();
}

dhrint(dev)
{
	register struct tty *tp;
	register c;
	register struct device *rp;

	sysinfo.rcvint++;
	rp = dh_addr[dev];
	while ((c = rp->dhnxch) < 0) {	/* char. present */
		tp = &dh_tty[((c>>8)&017)|(dev<<4)];
		if (tp >= &dh_tty[dh_cnt])
			continue;
		if (!(tp->t_state&(ISOPEN|WOPEN)))
			continue;
		(*linesw[tp->t_line].l_input)(tp, c, 0);
	}
}

/*
 * DH11 transmitter interrupt.
 * Restart each line which used to be active but has
 * terminated transmission since the last interrupt.
 */
dhxint(dev)
{
	register struct tty *tp;
	register ttybit, bar;
	struct device *rp;
	short *sbar;

	sysinfo.xmtint++;
	rp = dh_addr[dev];
	rp->dhcsr &= ~XINT;
	rp->dhcsr |= CLRNEM;
	sbar = &dhsar[dev];
	bar = *sbar & ~rp->dhbar;
	dev <<= 4;
	ttybit = 1;
	for (; bar; dev++, ttybit <<= 1) if (bar&ttybit) {
		tp = &dh_tty[dev];
		bar &= ~ttybit;
		lobyte(rp->dhcsr) = (dev&017) | IENAB;
		if (tp->t_state&BUSY) {
			register n;

			tp->t_state &= ~BUSY;
			n = rp->dhcar - &tp->t_buf->c_data[tp->t_buf->c_first];
			tp->t_buf->c_first += n;
		}
		if (tp->t_state&TTXON) {
			tp->t_state &= ~TTXON;
			rp->dhcar = &dh_xon;
			rp->dhbcr = -1;
			rp->dhbar |= ttybit;
			continue;
		}
		if (tp->t_state&TTXOFF) {
			tp->t_state &= ~TTXOFF;
			rp->dhcar = &dh_xoff;
			rp->dhbcr = -1;
			rp->dhbar |= ttybit;
			continue;
		}
		*sbar &= ~ttybit;
		dhproc(tp, T_OUTPUT);
	}
}

dhproc(tp, cmd)
register struct tty *tp;
{
	register struct device *rp;
	register n;
	char	*cp;
	int	dev, ttybit;
	extern ttrstrt();

	dev = tp - dh_tty;
	rp = dh_addr[dev>>4];
	ttybit = 1 << (dev&017);
	switch(cmd) {

	case T_TIME:
		tp->t_state &= ~TIMEOUT;
		rp->dhbrk &= ~ttybit;
		goto start;

	case T_WFLUSH:
	case T_RESUME:
		tp->t_state &= ~TTSTOP;
		goto start;

	case T_OUTPUT:
	start:
		if (tp->t_state&(TIMEOUT|TTSTOP|BUSY))
			break;
		if (tp->t_buf && tp->t_buf->c_first >= tp->t_buf->c_last) {
			putcf(tp->t_buf);
			tp->t_buf = NULL;
		}
		if (tp->t_buf == NULL) {
			if ((tp->t_buf = getcb(&tp->t_outq)) == NULL) {
				if (tp->t_state&TTIOW) {
					tp->t_state &= ~TTIOW;
					wakeup((caddr_t)&tp->t_oflag);
				}
				goto dd;
			}
		}
		n = tp->t_buf->c_last - tp->t_buf->c_first;
		cp = &tp->t_buf->c_data[tp->t_buf->c_first];
		if (tp->t_oflag&OPOST) {
			register char *p;
	
			p = cp;
			do {
				if (((*p++)&0377) == 0200) {
					p--;
					break;
				}
			} while(--n);
			n = p - cp;
		}
		if (n == 0) {
			if ((tp->t_buf->c_first+1) == tp->t_buf->c_last) {
				n = getc(&tp->t_outq);
				*cp = n;
			} else {
				tp->t_buf->c_first++;
				cp++;
				n = *cp & 0377;
			}
			if (n > 0200) {
				tp->t_buf->c_first++;
				timeout(ttrstrt, tp, (n&0177)+6);
				tp->t_state |= TIMEOUT;
				goto dd;
			}
			n = 1;
		}
		lobyte(rp->dhcsr) = (dev&017) | IENAB;
		rp->dhcar = cp;
		rp->dhbcr = -n;
		rp->dhbar |= ttybit;
		dhsar[dev>>4] |= ttybit;
		tp->t_state |= BUSY;
	dd:
		if (tp->t_state&OASLP &&
		    tp->t_outq.c_cc <= ttlowat[tp->t_cflag&CBAUD]) {
			tp->t_state &= ~OASLP;
			wakeup((caddr_t)&tp->t_outq);
		}
		break;

	case T_SUSPEND:
		tp->t_state |= TTSTOP;
		if (tp->t_state&BUSY) {
			lobyte(rp->dhcsr) = (dev&017) | IENAB;
			rp->dhbcr = -1;
		}
		break;

	case T_BLOCK:
		tp->t_state |= TBLOCK;
		tp->t_state &= ~TTXON;
		lobyte(rp->dhcsr) = (dev&017) | IENAB;
		if (tp->t_state&BUSY) {
			tp->t_state |= TTXOFF;
			rp->dhbcr = -1;
		} else {
			rp->dhcar = &dh_xoff;
			rp->dhbcr = -1;
			rp->dhbar |= ttybit;
			dhsar[dev>>4] |= ttybit;
		}
		break;

	case T_RFLUSH:
		if (!(tp->t_state&TBLOCK))
			break;

	case T_UNBLOCK:
		tp->t_state &= ~(TTXOFF|TBLOCK);
		lobyte(rp->dhcsr) = (dev&017) | IENAB;
		if (tp->t_state&BUSY) {
			tp->t_state |= TTXON;
			rp->dhbcr = -1;
		} else {
			rp->dhcar = &dh_xon;
			rp->dhbcr = -1;
			rp->dhbar |= ttybit;
			dhsar[dev>>4] |= ttybit;
		}
		break;

	case T_BREAK:
		rp->dhbrk |= ttybit;
		tp->t_state |= TIMEOUT;
		timeout(ttrstrt, tp, HZ/4);
		break;
	}
}

/*
 * Dump control bits into the DM registers.
 */
dmctl(dev, bits)
register dev;
{
	register struct devdm *dmaddr;
	register svcsr;

	sysinfo.mdmint++;
	dmaddr = dm_addr[dev>>4];
	dmaddr->dmcsr &= ~SCENABL;
	while((svcsr=dmaddr->dmcsr)&BSY);
	dmaddr->dmcsr = dev&017;
	dmaddr->dmlstat = bits;
	dev = dmaddr->dmlstat;
	dmaddr->dmcsr = svcsr|IENAB|SCENABL;
	return(dev);
}

/*
 * DM11 interrupt.
 * Mainly, deal with carrier transitions.
 */
dmintr(dev)
{
	register struct tty *tp;
	register struct devdm *dmaddr;
	register unit;

	dmaddr = dm_addr[dev];
	unit = (dmaddr->dmcsr&017)|(dev<<4);
	if (unit>=dh_cnt) {
		dmaddr->dmlstat = 0;
		dmaddr->dmcsr &= ~DONE;
		return;
	}
	tp = &dh_tty[unit];
	dm_inter[unit].cnt++;
	if (tp->t_cflag&CLOCAL) {
		dmaddr->dmlstat = 0;
		dmaddr->dmcsr &= ~DONE;
		return;
	}
	if (dmaddr->dmlstat&CARRIER) {
		if ((tp->t_state&CARR_ON)==0) {
			wakeup(&tp->t_canq);
			tp->t_state |= CARR_ON;
		}
	} else {
		if (tp->t_state&CARR_ON) {
			if (tp->t_state&ISOPEN) {
				signal(tp->t_pgrp, SIGHUP);
				dmaddr->dmlstat = 0;
				ttyflush(tp, FREAD|FWRITE);
			}
			tp->t_state &= ~CARR_ON;
		}
	}
	dmaddr->dmcsr &= ~DONE;
}

dhclr()
{
	register dev;
	register struct tty *tp;

	for (dev = 0; dev < dh_cnt; dev++) {
		tp = &dh_tty[dev];
		if ((tp->t_state&(ISOPEN|WOPEN)) == 0)
			continue;
		dhparam(dev);
		dmctl(dev, TURNON);
		tp->t_state &= ~BUSY;
		dhproc(tp, T_OUTPUT);
	}
}
