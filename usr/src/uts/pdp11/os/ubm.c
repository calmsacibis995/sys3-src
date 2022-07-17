#include "sys/param.h"
#include "sys/seg.h"
#include "sys/map.h"
#include "sys/buf.h"
#include "sys/systm.h"
#include "sys/var.h"

#define	btos(X)	((long)X + 8191) >> 13

struct map ubmap[12];
static ubmflag;
static paddr_t bufoffs;

ubmalloc(size)
unsigned size;
{
	register nseg, base;

	if (cputype != 70)
		return(0);
	nseg = btos(size);
	spl6();
	while ((base = malloc(ubmap, nseg)) == NULL) {
		ubmflag = 1;
		sleep(ubmap, PSWP+1);
	}
	spl0();
	return(base);
}

static
ubmload(base, addr, size)
union { short w[2]; long l; } addr;
unsigned size;
{
	register nseg;
	register short *up;

	nseg = btos(size);
	up = (short *)(&UBMAP->r[2*base]);
	while (nseg--) {
		*up++ = addr.w[1];
		*up++ = addr.w[0];
		addr.l += 8192;
	}
}

ubmfree(base, size)
unsigned size;
{
	register nseg;

	if (cputype != 70)
		return;
	nseg = btos(size);
	mfree(ubmap, nseg, base);
	if (ubmflag) {
		wakeup(ubmap);
		ubmflag = 0;
	}
}

ubminit()
{
#define	ntob(X)	((X-1)<<13)+1
#define	SSR3	((physadr)0172516)
#define	UBMON	040

	if (cputype != 70)
		return;
	ubmload(0, 0L, ntob(6));	/* Kernel D space */
	ubmload(6, bufbase, ntob(8));	/* buffers */
	bufoffs = bufbase - (6L << 13);
	mfree(ubmap, 31-14, 14);
	SSR3->r[0] |= UBMON;
}

paddr_t
ubmaddr(bp, base)
register struct buf *bp;
{

	if (cputype != 70)
		return(bp->b_paddr);
	if (bp->b_flags&B_PHYS) {
		ubmload(base, bp->b_paddr, bp->b_bcount);
		bp->b_flags |= B_MAP;
		return((paddr_t)base<<13);
	}
	if (bp->b_paddr >= (paddr_t)(unsigned)sabuf[0]
	 && bp->b_paddr <= (paddr_t)(unsigned)sabuf[v.v_sabuf])
		return(bp->b_paddr);
	return(bp->b_paddr - bufoffs);
}
