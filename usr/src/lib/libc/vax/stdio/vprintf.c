#include <stdio.h>
#include <varargs.h>

int
vprintf (fmt, args)
	char *fmt;
	va_list args;
{
	return _doprnt (fmt, args, stdout);
}
