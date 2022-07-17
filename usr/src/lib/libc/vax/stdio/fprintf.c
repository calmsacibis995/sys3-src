/*LINTLIBRARY*/
#include	<stdio.h>

/*VARARGS2*/
fprintf(iop, fmt, args)
FILE *iop;
char *fmt;
{
	return(_doprnt(fmt, &args, iop));
}
