#include <stdio.h>
#include <varargs.h>

int
vfprintf (file, fmt, args)
	FILE *file;
	char *fmt;
	va_list args;
{
	return _doprnt (fmt, args, file);
}
