/*
 *	Console/LSI interface
 */
#include "sys/param.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/tty.h"
#include "sys/buf.h"
#include "sys/systm.h"
#include "sys/cons.h"
#include "sys/conf.h"
#include "sys/sysinfo.h"
#include "sys/mtpr.h"

struct	tty con_tty;

struct	floppy	{
	struct buf *	f_buf;
	unsigned char *	f_ptr;
	short	f_cnt;
	short	f_sec;
	short	f_trk;
	char	f_state;
	char	f_errcnt;
} con_flp;

int	conproc();
int	ttrstrt();
char	partab[];

#define	CONSOLE	0
#define	FLOPPY	1

#define	FSIDLE	1
#define	FSRWC	3
#define	FSSEC	4
#define	FSTRK	5
#define	FSRDATA	6
#define	FSXDATA	7
#define	FSDONE	8
#define	FSABORT	10

conopen(dev, flag)
dev_t dev;
{
	if (dev == CONSOLE) {
		register struct tty *tp;
	
		tp = &con_tty;
		tp->t_proc = conproc;
		if ((tp->t_state&ISOPEN) == 0) {
			ttinit(tp);
			tp->t_state = ISOPEN|CARR_ON;
			tp->t_iflag |= ICRNL;
			tp->t_oflag |= OPOST|ONLCR|TAB3;
			tp->t_lflag |= ISIG|ICANON|ECHO|ECHOK;
			mtpr(RXCS, C_IE);
			mtpr(TXCS, C_IE);
		}
		(*linesw[tp->t_line].l_open)(tp);
	} else if (dev == FLOPPY) {
		register struct floppy *fp;

		fp = &con_flp;
		if (fp->f_state)
			u.u_error = ENXIO;
		else {
			fp->f_state = FSIDLE;
			fp->f_buf = geteblk();
			fp->f_buf->b_flags = 0;
		}
	} else
		u.u_error = ENXIO;
}

conclose(dev)
dev_t dev;
{
	if (dev == CONSOLE) {
		register struct tty *tp;

		tp = &con_tty;
		(*linesw[tp->t_line].l_close)(tp);
		tp->t_state = 0;
	} else if (dev == FLOPPY) {
		brelse(con_flp.f_buf);
		con_flp.f_state = 0;
	}
}

conread(dev)
dev_t dev;
{
	if (dev == CONSOLE) {
		ttread(&con_tty);
	} else if (dev == FLOPPY) {
		confpio(B_READ);
	}
}

conwrite(dev)
dev_t dev;
{
	if (dev == CONSOLE) {
		ttwrite(&con_tty);
	} else if (dev == FLOPPY) {
		confpio(B_WRITE);
	}
}

conxint(dev)
{
	register struct tty *tp;

	sysinfo.xmtint++;
	tp = &con_tty;
	if (mfpr(TXCS) & C_DONE) {
		tp->t_state &= ~BUSY;
		conproc(tp, T_OUTPUT);
	}
}

conrint(dev)
{
	register struct tty *tp;
	register c;

	sysinfo.rcvint++;
	tp = &con_tty;
	while (mfpr(RXCS) & C_DONE) {
		c = mfpr(RXDB);
		if ((c & C_CID) == 0)
			(*linesw[tp->t_line].l_input)(tp, c, 0);
		else
			confrcv(c);
	}
}

conioctl(dev, cmd, arg, mode)
{
	if (dev == CONSOLE)
		ttiocom(&con_tty, cmd, arg, mode);
}

conproc(tp, cmd)
register struct tty *tp;
{
	register c;

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
			mtpr(TXDB, c & C_DATA);
			break;
		}
		if (tp->t_state&OASLP && tp->t_outq.c_cc<=ttlowat[tp->t_cflag&CBAUD]) {
			tp->t_state &= ~OASLP;
			wakeup((caddr_t)&tp->t_outq);
		}
		if ((tp->t_state&BUSY) == 0 && con_flp.f_state > FSIDLE)
			confxmt();
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

	while((mfpr(TXCS) & C_DONE) == 0)
		;
	if(c == 0)
		return;
	s = mfpr(TXCS);
	mtpr(TXCS,0);
	mtpr(TXDB, c & C_DATA);
	if(c == '\n') {
		putchar('\r');
	}
	putchar(0);
	mtpr(TXCS, s);
}

confpio(rw)
{
	register struct floppy *fp = &con_flp;
	register struct buf *bp;
	register cnt;

	bp = fp->f_buf;
	spl4();
	while (bp->b_flags&B_BUSY)
		sleep((caddr_t)&con_flp, PRIBIO);
	bp->b_flags = B_BUSY;
	spl0();
	while (u.u_count) {
		bp->b_blkno = u.u_offset/FSIZE;
		if (bp->b_blkno >= (FTRK-1)*FSEC || u.u_offset%FSIZE) {
			u.u_error = ENXIO;
			break;
		}
		cnt = min(FSIZE, u.u_count);
		if (rw == B_WRITE) {
			iomove(bp->b_un.b_addr, cnt, B_WRITE);
			if (u.u_error)
				break;
		}
		fp->f_errcnt = 0;
		bp->b_flags |= rw;
		bp->b_flags &= ~B_DONE;
		{
			register sec,trk;

			trk = bp->b_blkno/FSEC;
			sec = bp->b_blkno%FSEC;
			sec = (sec*2 + (sec>=13) + trk*6)%26;
			fp->f_trk = F_DATA | (trk + 1);
			fp->f_sec = F_DATA | (sec + 1);
		}
		spl4();
		fp->f_state = FSRWC;
		conproc(&con_tty, T_OUTPUT);
		while ((bp->b_flags&B_DONE) == 0)
			sleep((caddr_t)bp, PRIBIO);
		spl0();
		if (bp->b_flags&B_ERROR) {
			u.u_error = EIO;
			break;
		}
		if (rw == B_READ) {
			iomove(bp->b_un.b_addr, cnt, B_READ);
			if (u.u_error)
				break;
		}
	}
	bp->b_flags = 0;
	wakeup((caddr_t)&con_flp);
}

confxmt()
{
	register struct floppy *fp = &con_flp;
	register struct buf *bp;

	bp = fp->f_buf;
	switch(fp->f_state) {
	case FSRWC:
		fp->f_cnt = FSIZE;
		fp->f_ptr = (unsigned char *)bp->b_un.b_addr;
		mtpr(TXDB, bp->b_flags&B_READ?F_READ:F_WRITE);
		fp->f_state = FSSEC;
		break;
	case FSSEC:
		mtpr(TXDB, fp->f_sec);
		fp->f_state = FSTRK;
		break;
	case FSTRK:
		mtpr(TXDB, fp->f_trk);
		fp->f_state = bp->b_flags&B_READ?FSDONE:FSXDATA;
		break;
	case FSXDATA:
		mtpr(TXDB, F_DATA|(int)*fp->f_ptr++);
		if (--fp->f_cnt == 0)
			fp->f_state = FSDONE;
		break;
	case FSABORT:
		mtpr(TXDB, F_CAN);
		if (++fp->f_errcnt <= 8) {
			fp->f_state = FSRWC;
		} else {
			bp->b_flags |= B_ERROR|B_DONE;
			fp->f_state = FSIDLE;
			wakeup(bp);
		}
		break;
	default:
		return;
	}
	con_tty.t_state |= BUSY;
}

confrcv(c)
register c;
{
	register struct floppy *fp = &con_flp;
	register struct buf *bp;

	bp = fp->f_buf;
	switch (c & C_CID) {
	case F_DATA:
		if (fp->f_state != FSRDATA) {
			fp->f_state = FSABORT;
			break;
		}
		*fp->f_ptr++ = c & C_DATA;
		if (--fp->f_cnt == 0) {
			fp->f_state = FSIDLE;
			bp->b_flags |= B_DONE;
			wakeup(bp);
		}
		break;
	case F_FFC:
		if (fp->f_state != FSDONE || (c & C_DATA)) {
			fp->f_state = FSABORT;
			break;
		}
		if (bp->b_flags&B_READ) {
			fp->f_state = FSRDATA;
		} else {
			fp->f_state = FSIDLE;
			bp->b_flags |= B_DONE;
			wakeup(bp);
		}
		break;
	case F_FLP:
		fp->f_state = FSABORT;
		break;
	}
	if (fp->f_state == FSABORT)
		conproc(&con_tty, T_OUTPUT);
}

conclr()
{
	register struct tty *tp;

	tp = &con_tty;
	if((tp->t_state&ISOPEN) == 0)
		return;
	mtpr(RXCS, C_IE);
	mtpr(TXCS, C_IE);
	tp->t_state &= ~BUSY;
	conproc(tp, T_OUTPUT);
}
