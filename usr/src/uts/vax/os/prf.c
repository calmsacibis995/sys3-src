#include "sys/param.h"
#include "sys/systm.h"
#include "sys/buf.h"
#include "sys/elog.h"
#include "sys/iobuf.h"

/*
 * In case console is off,
 * panicstr contains argument to last call to panic.
 */
char	*panicstr;

/*
 * Scaled down version of C Library printf.
 * Only %s %u %d (==%u) %o %x %D are recognized.
 * Used to print diagnostic information
 * directly on console tty.
 * Since it is not interrupt driven,
 * all system activities are pretty much suspended.
 * Printf should not be used for chit-chat.
 */
printf(fmt, x1)
register char *fmt;
unsigned x1;
{
	register c;
	register unsigned int *adx;
	char *s;

	adx = &x1;
loop:
	while((c = *fmt++) != '%') {
		if(c == '\0')
			return;
		putchar(c);
	}
	c = *fmt++;
	if(c == 'd' || c == 'u' || c == 'o' || c == 'x')
		printn((long)*adx, c=='o'? 8: (c=='x'? 16:10));
	else if(c == 's') {
		s = (char *)*adx;
		while(c = *s++)
			putchar(c);
	} else if (c == 'D') {
		printn(*(long *)adx, 10);
		adx += (sizeof(long) / sizeof(int)) - 1;
	}
	adx++;
	goto loop;
}

printn(n, b)
long n;
register b;
{
	register i, nd, c;
	int	flag;
	int	plmax;
	char d[12];

	c = 1;
	flag = n < 0;
	if (flag)
		n = (-n);
	if (b==8)
		plmax = 11;
	else if (b==10)
		plmax = 10;
	else if (b==16)
		plmax = 8;
	if (flag && b==10) {
		flag = 0;
		putchar('-');
	}
	for (i=0;i<plmax;i++) {
		nd = n%b;
		if (flag) {
			nd = (b - 1) - nd + c;
			if (nd >= b) {
				nd -= b;
				c = 1;
			} else
				c = 0;
		}
		d[i] = nd;
		n = n/b;
		if ((n==0) && (flag==0))
			break;
	}
	if (i==plmax)
		i--;
	for (;i>=0;i--) {
		putchar("0123456789ABCDEF"[d[i]]);
	}
}

/*
 * Panic is called on unresolvable fatal errors.
 * It syncs, prints "panic: mesg" and then loops.
 */
panic(s)
char *s;
{
	panicstr = s;
	update();
	printf("panic: %s\n", s);
	for(;;)
		idle();
}

/*
 * prdev prints a warning message of the
 * form "mesg on dev x/y".
 * x and y are the major and minor parts of
 * the device argument.
 */
prdev(str, dev)
char *str;
dev_t dev;
{

	printf("%s on dev %u/%u\n", str, major(dev), minor(dev));
}

static char *nam[] = {
	"RK05",
	"RP03",
	"RF11",
	"TM11",
	"TC11",
	"RP04/5/6",
	"TM02/3",
	"RS03/4",
	"RL01/2"
};
/*
 * Deverr prints a diagnostic from a device driver.
 * It prints the device, block number,
 * and two arguments, usually error status.
 */
deverr(dp, o1, o2)
register struct iobuf *dp;
{
	register struct buf *bp;

	bp = dp->b_actf;
	printf("err on %s, minor %o\n", nam[major(dp->b_dev)], minor(bp->b_dev));
	printf("bn=%D er=%o,%o\n", bp->b_blkno, o1, o2);
}
