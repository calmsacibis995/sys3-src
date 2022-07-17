/*
 *   KL/DL-11 driver
 */
#include "sys/param.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/tty.h"
#include "sys/conf.h"
#include "sys/sysinfo.h"

struct device *kl_addr[];
int	kl_cnt;
struct tty kl_tty[];
int	klproc(), ttrstrt();
char	partab[];
static	diag_out	= 1;

#define DSRDY	02
#define	RDRENB	01

struct device {
	short rcsr, rbuf;
	short tcsr, tbuf;
};

klopen(dev, flag)
{
	register struct device *addr;
	register struct tty *tp;

	if(dev >= kl_cnt) {
		u.u_error = ENXIO;
		return;
	}
	tp = &kl_tty[dev];
	addr = kl_addr[dev];
	tp->t_proc = klproc;
	if ((tp->t_state&ISOPEN) == 0) {
		ttinit(tp);
		tp->t_state = ISOPEN|CARR_ON;
		tp->t_iflag |= ICRNL|ISTRIP;
		tp->t_oflag |= OPOST|ONLCR|TAB3;
		tp->t_lflag |= ISIG|ICANON|ECHO|ECHOK;
	}
	addr->rcsr |= IENABLE|DSRDY|RDRENB;
	addr->tcsr |= IENABLE;
	(*linesw[tp->t_line].l_open)(tp);
}

klclose(dev)
{
	register struct tty *tp;

	tp = &kl_tty[dev];
	(*linesw[tp->t_line].l_close)(tp);
	tp->t_state = 0;
}

klread(dev)
{
	register struct tty *tp;

	tp = &kl_tty[dev];
	(*linesw[tp->t_line].l_read)(tp);
}

klwrite(dev)
{
	register struct tty *tp;

	tp = &kl_tty[dev];
	(*linesw[tp->t_line].l_write)(tp);
}

klxint(dev)
{
	register struct tty *tp;
	register struct device *addr;

	sysinfo.xmtint++;
	tp = &kl_tty[dev];
	addr = kl_addr[dev];
	if (addr->tcsr&DONE) {
		tp->t_state &= ~BUSY;
		klproc(tp, T_OUTPUT);
	}
}

klrint(dev)
{
	register int c;
	register struct device *addr;
	register struct tty *tp;

	sysinfo.rcvint++;
	tp = &kl_tty[dev];
	addr = kl_addr[dev];
	c = addr->rbuf;
	addr->rcsr |= RDRENB;
	if(dev || (c&0177) != (037&'o'))
		(*linesw[tp->t_line].l_input)(tp, c, 0);
	else
		{
		if(diag_out)
			printf("\nO.S. messages -- OFF\n");
		diag_out = !diag_out;
		if(diag_out)
			printf("\nO.S. messages -- ON\n");
	}
}

klioctl(dev, cmd, arg, mode)
register dev;
{
	ttiocom(&kl_tty[dev], cmd, arg, mode);
}

klproc(tp, cmd)
register struct tty *tp;
{
	register c;
	register struct device *addr;

	switch (cmd) {

	case T_TIME:
		tp->t_state &= ~TIMEOUT;
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
					timeout(ttrstrt, (caddr_t)tp, (c&0177));
					break;
				}
			}
			tp->t_state |= BUSY;
	/* parity ? */
			addr = kl_addr[tp - kl_tty];
			addr->tbuf = c;
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
		tp->t_state |= TBLOCK;
		break;

	case T_RFLUSH:
	case T_UNBLOCK:
		tp->t_state &= ~TBLOCK;
		break;
	}
}

putchar(c)
register c;
{
	register s;
	register struct device *addr;

	addr = kl_addr[0];
	while((addr->tcsr&DONE) == 0)
		;
	if(c == 0)
		return;
	if(diag_out == 0)
		return;
	s = addr->tcsr;
	addr->tcsr = 0;
	addr->tbuf = c;
	if(c == '\n') {
		putchar('\r');
		putchar(0177);
		putchar(0177);
	}
	putchar(0);
	addr->tcsr = s;
}

klclr()
{
	register dev;
	register struct device *addr;
	register struct tty *tp;

	for (dev = 0; dev < kl_cnt; dev++) {
		tp = &kl_tty[dev];
		if ((tp->t_state&(ISOPEN|WOPEN)) == 0)
			continue;
		addr = kl_addr[dev];
		addr->rcsr |= IENABLE|DSRDY|RDRENB;
		addr->tcsr |= IENABLE;
		tp->t_state &= ~BUSY;
		klproc(tp, T_OUTPUT);
	}
}
