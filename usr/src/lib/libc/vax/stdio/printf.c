/*LINTLIBRARY*/
#include	<stdio.h>

/*VARARGS1*/
printf(fmt, args)
char *fmt;
{
	return(_doprnt(fmt, &args, stdout));
}
