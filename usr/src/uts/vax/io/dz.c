/*
 *	DZ11 driver
 */

#include "sys/param.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/file.h"
#include "sys/tty.h"
#include "sys/conf.h"
#include "sys/sysinfo.h"

struct device *dz_addr[];
int	dz_cnt;
struct tty dz_tty[];
short	dz_scan;

char	dz_brk[8];	/* break bits - max 64 lines */
char	dz_speeds[16] = {
	0, 00, 01, 02, 03, 04, 0, 05,
	06, 07, 010, 012, 014, 016, 0, 0
	};

#define	BITS6	010
#define	BITS7	020
#define	TWOSB	040
#define	PENABLE	0100
#define	OPAR	0200
#define	RCVENB	010000

#define	IE	040140
struct device {
	short	dzcsr, dzrbuf;
	char	dztcr, dzdtr;
	char	dztbuf, dzbrk;
};
#define	dzlpr	dzrbuf
#define	dzmsr	dzbrk

#define	ON	1
#define	OFF	0


dzopen(dev, flag)
{
	register struct tty *tp;
	extern dzproc(), dzscan();

	if (dev >= dz_cnt) {
		u.u_error = ENXIO;
		return;
	}
	tp = &dz_tty[dev];
	if ((tp->t_state&(ISOPEN|WOPEN)) == 0) {
		ttinit(tp);
		tp->t_proc = dzproc;
		dzparam(dev);
	}
	spl5();
	if (tp->t_cflag&CLOCAL || dzmodem(dev, ON))
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

dzclose(dev)
{
	register struct tty *tp;

	tp = &dz_tty[dev];
	(*linesw[tp->t_line].l_close)(tp);
	if (tp->t_cflag&HUPCL)
		dzmodem(dev, OFF);
}

dzread(dev)
{
	register struct tty *tp;

	tp = &dz_tty[dev];
	(*linesw[tp->t_line].l_read)(tp);
}

dzwrite(dev)
{
	register struct tty *tp;

	tp = &dz_tty[dev];
	(*linesw[tp->t_line].l_write)(tp);
}

dzioctl(dev, cmd, arg, mode)
register dev;
{
	switch(cmd) {
	default:
		if (ttiocom(&dz_tty[dev], cmd, arg, mode))
			dzparam(dev);
	}
}

dzparam(dev)
{
	register struct tty *tp;
	register flags, lpr;
	register struct device *dzaddr;

	tp = &dz_tty[dev];
	dzaddr= dz_addr[dev>>3];
	dzaddr->dzcsr = IE;
	if (dz_scan==0) {
		dzscan();
		dz_scan++;
	}
	flags = tp->t_cflag;
	if ((flags&CBAUD) == 0) {
		/* hang up line */
		dzmodem(dev, OFF);
		return;
	}
	lpr = (dz_speeds[flags&CBAUD]<<8)|(dev&07);
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
	dzaddr->dzlpr = lpr;
}

dzrint(dev)
{
	register struct tty *tp;
	register c;
	register line;
	register struct device *dzaddr;

	sysinfo.rcvint++;
	dzaddr = dz_addr[dev];
	while ((c = dzaddr->dzrbuf) < 0) {	/* char. present */
		line = (c>>8)&07;
		tp = &dz_tty[line|(dev<<3)];
		if (tp >= &dz_tty[dz_cnt])
			continue;
		if (!(tp->t_state&(ISOPEN|WOPEN)))
			continue;
		if (tp->t_cflag&CLOCAL || dzaddr->dzmsr&(1<<line)) {
			if ((tp->t_state&CARR_ON) == 0) {
				wakeup(&tp->t_canq);
				tp->t_state |= CARR_ON;
			}
		} else {
			if (tp->t_state&CARR_ON) {
				signal(tp->t_pgrp, SIGHUP);
				dzaddr->dzdtr &= ~(1<<line);
				tp->t_state &= ~CARR_ON;
				ttyflush(tp, (FREAD|FWRITE));
			}
			continue;
		}
		(*linesw[tp->t_line].l_input)(tp, c, 0);
	}
}

dzxint(dev)
{
	register struct tty *tp;
	register struct device *dzaddr;
	register unit;

	sysinfo.xmtint++;
	dzaddr = dz_addr[dev];
	while(dzaddr->dzcsr<0) {	/* TX rdy */
		unit = (dzaddr->dzcsr >> 8) & 07;
		tp = &dz_tty[(dev<<3)|unit];
		if (tp->t_state & TTXON) {
			dzaddr->dztbuf = CSTART;
			tp->t_state &= ~TTXON;
			continue;
		}
		if (tp->t_state & TTXOFF) {
			dzaddr->dztbuf = CSTOP;
			tp->t_state &= ~TTXOFF;
			continue;
		}
		if (tp->t_state & BUSY) {
			dzaddr->dztbuf = (char)tp->t_buf;
			tp->t_state &= ~BUSY;
			dzproc(tp, T_OUTPUT);
			continue;
		}
		dzaddr->dztcr &= ~(1<<unit);
	}
}

dzproc(tp, cmd)
register struct tty *tp;
{
	register unit, c;
	struct device *dzaddr;
	int	dev;
	extern ttrstrt();

	unit = tp - dz_tty;
	dev = unit>>3;
	dzaddr = dz_addr[dev];
	unit = 1<<(unit&07);
	switch(cmd) {

	case T_TIME:
		tp->t_state &= ~TIMEOUT;
		dz_brk[dev] &= ~unit;
		dzaddr->dzbrk = dz_brk[dev];
		goto start;

	case T_WFLUSH:
	case T_RESUME:
		tp->t_state &= ~TTSTOP;
		goto start;

	case T_OUTPUT:
	start:
		if (tp->t_state&(TIMEOUT|TTSTOP|BUSY))
			break;
		if (tp->t_state&TTIOW && tp->t_outq.c_cc==0) {
			tp->t_state &= ~TTIOW;
			wakeup((caddr_t)&tp->t_oflag);
		}
		while ((c=getc(&tp->t_outq)) >= 0) {
			if (tp->t_oflag&OPOST && c == 0200) {
				if ((c = getc(&tp->t_outq)) < 0)
					break;
				if (c > 0200) {
					tp->t_state |= TIMEOUT;
					timeout(ttrstrt, tp, (c&0177)+6);
					break;
				}
			}
			*((char *)&tp->t_buf) = c;
			tp->t_state |= BUSY;
			dzaddr->dztcr |= unit;
			break;
		}
		if (tp->t_state&OASLP && tp->t_outq.c_cc<=ttlowat[tp->t_cflag&CBAUD]) {
			tp->t_state &= ~OASLP;
			wakeup((caddr_t)&tp->t_outq);
		}
		break;

	case T_SUSPEND:
		tp->t_state |= TTSTOP;
		break;

	case T_BLOCK:
		tp->t_state &= ~TTXON;
		tp->t_state |= TTXOFF|TBLOCK;
		dzaddr->dztcr |= unit;
		break;

	case T_RFLUSH:
		if (!(tp->t_state&TBLOCK))
			break;
	case T_UNBLOCK:
		tp->t_state &= ~(TTXOFF|TBLOCK);
		tp->t_state |= TTXON;
		dzaddr->dztcr |= unit;
		break;

	case T_BREAK:
		dz_brk[dev] |= unit;
		dzaddr->dzbrk = dz_brk[dev];
		tp->t_state |= TIMEOUT;
		timeout(ttrstrt, tp, HZ/4);
		break;
	}
}

dzmodem(dev, flag)
{
	register struct device *dzaddr;
	register bit;

	dzaddr = dz_addr[dev>>3];
	bit = 1<<(dev&07);
	if (flag==OFF)
		dzaddr->dzdtr &= ~bit;
	else
		dzaddr->dzdtr |= bit;
	return(dzaddr->dzmsr & bit);
}

dzscan()
{
	register i;
	register struct device *dzaddr;
	register struct tty *tp;
	char	bit;

	for (i=0; i<dz_cnt; i++) {
		tp = &dz_tty[i];
		if (!(tp->t_state&(ISOPEN|WOPEN)))
			continue;
		dzaddr = dz_addr[i>>3];
		bit = 1<<(i&07);
		if (tp->t_cflag&CLOCAL || dzaddr->dzmsr&bit) {
			if ((tp->t_state&CARR_ON)==0) {
				wakeup(&tp->t_canq);
				tp->t_state |= CARR_ON;
			}
		} else {
			if (tp->t_state&CARR_ON) {
				if (tp->t_state&ISOPEN) {
					signal(tp->t_pgrp, SIGHUP);
					dzaddr->dzdtr &= ~bit;
					ttyflush(tp, (FREAD|FWRITE));
				}
				tp->t_state &= ~CARR_ON;
			}
		}
	}
	timeout(dzscan, 0, 120);
}

dzclr()
{
	register dev;
	register struct tty *tp;

	for (dev = 0; dev < dz_cnt; dev++) {
		tp = &dz_tty[dev];
		if ((tp->t_state&(ISOPEN|WOPEN)) == 0)
			continue;
		dzparam(dev);
		dzmodem(dev, ON);
		tp->t_state &= ~BUSY;
		dzproc(tp, T_OUTPUT);
	}
}
