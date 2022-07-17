static char	sccsid[] = "@(#)tty.c	4.9";

# include "saio.h"
# include <sgtty.h>

# define TTYHOG 256

#ifdef vax
#define RXCS 32		/* console receiver control and status */
#define RXDB 33		/* console receiver data buffer */
#define TXCS 34		/* console transmitter control and status */
#define TXDB 35		/* console transmitter data buffer */
#endif

#ifdef pdp11
struct device {
	int	rcsr;
	int	rbuf;
	int	tcsr;
	int	tbuf;
};

#define KLADDR	((struct device *) 0177560)
#endif

struct sgttyb _ttstat = { B300, B300, '#', '@', CRMOD|ECHO|XTABS };

static struct {
	char	*t_fptr;
	char	*t_lptr;
	char	t_buf[TTYHOG];
} _tt = { _tt.t_buf, _tt.t_buf };

static
_ttputc (c)
register c; {
	register s;
	register unsigned timo;

	/*
	 *  If last char was a break or null, don't print
	 */
#ifdef vax
	if ((_mfpr (RXCS) & 0200) && (_mfpr (RXDB) & 0177) == 0177) {
#endif
#ifdef pdp11
	if ((KLADDR->rcsr & 0200) && (KLADDR->rbuf & 0177) == 0177) {
#endif
		_ttcanon ('\n');
		_exit ();
	}
	timo = (unsigned) 60000;
	/*
	 * Try waiting for the console tty to come ready,
	 * otherwise give up after a reasonable time.
	 */
#ifdef vax
	while ((_mfpr (TXCS) & 0200) == 0)
#endif
#ifdef pdp11
	while ((KLADDR->tcsr & 0200) == 0)
#endif
		if (--timo == 0)
			break;
	if (c == 0)
		return;
#ifdef vax
	s = _mfpr (TXCS);
	_mtpr (TXCS, 0);
	_mtpr (TXDB, c);
	_ttputc (0);
	_mtpr (TXCS, s);
#endif
#ifdef pdp11
	s = KLADDR->tcsr;
	KLADDR->tcsr = 0;
	KLADDR->tbuf = c;
	_ttputc (0);
	KLADDR->tcsr = s;
#endif
}

static
_ttcanon (c)
char c; {
	register lcc, flag;
	static mcc;

	flag = _ttstat.sg_flags;

	switch (c) {

	case '\0':
		return;

	case '\b':
		if (mcc <= 0)
			return;
		_ttputc ('\b');
		--mcc;
		return;
	
	case '\t':
		if (flag & XTABS) {
			lcc = (mcc + 8) & ~7;
			while (mcc++ < lcc)
				_ttputc (' ');
		} else
			_ttputc ('\t');
		return;
	
	case '\n':
		_ttputc ('\n');
		if ((flag & CRMOD) == 0)
			return;
	case '\r':
		_ttputc ('\r');
#ifdef pdp11
		_ttputc (0177);
		_ttputc (0177);
#endif
		mcc = 0;
		return;
	}

	if (c >= ' ' && c < 0177) {
		_ttputc (c);
		++mcc;
	}
}

_ttread (buf, n)
char *buf; {
	register c, cnt, flag;

	if ((cnt = _tt.t_lptr - _tt.t_fptr) == 0) {
		flag = _ttstat.sg_flags;
		for (;;) {
#ifdef vax
			while ((_mfpr (RXCS) & 0200) == 0);
			c = _mfpr (RXDB) & 0177;
#endif
#ifdef pdp11
			KLADDR->rcsr = 1;
			while ((KLADDR->rcsr & 0200) == 0);
			c = KLADDR->rbuf & 0177;
#endif
			if (c == '\r' && (flag & CRMOD))
				c = '\n';
			if (flag & ECHO)
				_ttcanon (c);

			if (flag & LCASE) {
				if (c>='A' && c<='Z')
					c -= 'A' - 'a';
				if (_tt.t_lptr > _tt.t_fptr && _tt.t_lptr[-1] == '\\') {
					_tt.t_lptr--;
					if (c>='a' && c<='z')
						c += 'A' - 'a';
					else
						switch (c) {
						case '(':
							c = '{';
							break;
						case ')':
							c = '}';
							break;
						case '!':
							c = '|';
							break;
						case '^':
							c = '~';
							break;
						case '\'':
							c = '`';
							break;
						}
				}
			}
			cnt = _tt.t_lptr - _tt.t_fptr;
			if ((flag & RAW) == 0
			    && (_tt.t_lptr[-1] != '\\' || _tt.t_lptr == _tt.t_fptr)) {
				if (c == _ttstat.sg_erase) {
					_tt.t_lptr--;
					if (_tt.t_lptr < _tt.t_fptr)
						_tt.t_lptr = _tt.t_fptr;
					continue;
				}
				if (c == _ttstat.sg_kill) {
					_tt.t_lptr = _tt.t_fptr;
					if (flag & ECHO)
						_ttcanon ('\n');
					continue;
				}
				if (c == 04)
					break;
			}
			*_tt.t_lptr++ = c;
			++cnt;
			if (flag & RAW)
				if (cnt >= n)
					break;
				else;
			else
				if (c == '\n')
					break;
			if (_tt.t_lptr < &_tt.t_buf[TTYHOG-1])
				continue;
			if (_tt.t_fptr == _tt.t_buf) {
				_tt.t_buf[TTYHOG-1] = '\n';
				_ttcanon ('\n');
				break;
			}
			strncpy (_tt.t_buf, _tt.t_fptr, cnt);
			_tt.t_lptr = &_tt.t_buf[cnt];
			_tt.t_fptr = _tt.t_buf;
		}
	}
	cnt = cnt>n ? n : cnt;
	strncpy (buf, _tt.t_fptr, cnt);
	if (n < cnt)
		_tt.t_fptr += cnt;
	else
		_tt.t_fptr = _tt.t_lptr = _tt.t_buf;
	return (cnt);
}

_ttwrite (buf, n)
char *buf; {
	register cnt;
	register char *ptr;

	ptr = buf;

	for (cnt=0; cnt<n; cnt++)
		_ttcanon (*ptr++);

	return (n);
}

_prs (str)
char *str; {

	_ttwrite (str, strlen (str));
}