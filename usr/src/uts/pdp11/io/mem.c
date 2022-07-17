/*
 *	Memory special file
 *	minor device 0 is physical memory
 *	minor device 1 is kernel memory
 *	minor device 2 is EOF/NULL
 */

#include "sys/param.h"
#include "sys/dir.h"
#include "sys/user.h"

mmread(dev)
{
	register unsigned n;

	do {
		n = min(u.u_count, BSIZE);
		if (dev == 0) {
			if (copyio(u.u_offset, u.u_base, n, U_RUD))
				u.u_error = ENXIO;
		} else if (dev == 1) {
			if (copyout((short)u.u_offset, u.u_base, n))
				u.u_error = ENXIO;
		} else
			return;
		u.u_offset += n;
		u.u_base += n;
		u.u_count -= n;
	} while(u.u_error==0 && u.u_count!=0);
}

mmwrite(dev)
{
	register unsigned n;

	while(u.u_error==0 && u.u_count!=0) {
		n = min(u.u_count, BSIZE);
		if (dev == 0) {
			if (copyio(u.u_offset, u.u_base, n, U_WUD))
				u.u_error = ENXIO;
		} else if (dev == 1) {
			if (copyin(u.u_base, (short)u.u_offset, n))
				u.u_error = ENXIO;
		}
		u.u_offset += n;
		u.u_base += n;
		u.u_count -= n;
	}
}
