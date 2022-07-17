/*LINTLIBRARY*/
#include	<stdio.h>

/*VARARGS2*/
sprintf(str, fmt, args)
char *str, *fmt;
{
	register int n;
	struct _iobuf _strbuf;

	_strbuf._flag = _IOWRT+_IOSTRG;
	_strbuf._ptr = str;
	_strbuf._cnt = 0x7fffffff;
	n = _doprnt(fmt, &args, &_strbuf);
	putc('\0', &_strbuf);
	return(n);
}
