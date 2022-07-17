/*LINTLIBRARY*/
/*
 *	abort() - terminate current process with dump via SIGIOT
 */

#include <signal.h>

int getpid();

abort()
{
	return kill (getpid(), SIGIOT);
}
