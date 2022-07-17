
#include <stdio.h>
#include <varargs.h>

int
vsprintf (str, fmt, args)
	char *str, *fmt;
	va_list args;
{
	register int n;
	struct _iobuf _strbuf;
	_strbuf._flag = _IOWRT+_IOSTRG;
	_strbuf._ptr = str;
	_strbuf._cnt = 0x7fffffff;
	n = _doprnt (fmt, args, stdout);
	putc ('\0', &_strbuf);
	return n;
}
