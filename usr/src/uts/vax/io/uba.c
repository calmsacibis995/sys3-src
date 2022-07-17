#include "sys/param.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/uba.h"
#include "sys/map.h"
#include "sys/buf.h"
#include "sys/page.h"

struct map ubmap[UAMSIZ];
struct map bdpmap[15];
int	ubazero;
static char bdpwant, ubmwant;
paddr_t	ubcoffs;
extern	_sdata, end;

ubmalloc(size, bdpflg)
unsigned size;
{
/*
 * Allocate as many contiguous UBA mapping registers
 * as are necessary to do transfer of 'size' bytes.
 * Wait for enough map registers.
 * if 'bdpflg' is non-zero: a buffered data path (BDP) is used;
 * else a direct data path (DDP).
 * Return
 *   |21 - - 18|17 - -  9|8  - -  0|
 *   |  BDP #  |    #    |  start  |
 *   |         | map reg | map reg |
 */
	register nseg, base;
	register bdp = 0;
	 
	nseg = btoc(size) + 2;
	spl6();
	while ((base = malloc(ubmap, nseg)) == NULL) {
		ubmwant++;
		sleep(ubmap, PSWP);
	}
	if (bdpflg)
		while ( (bdp=malloc(bdpmap, 1)) == NULL) {
			bdpwant++;
			sleep(bdpmap, PSWP);
		}
	spl0();
	return((bdp<<18) | (nseg<<9) | base);
}
 
ubmfree(umd)
{
	register nseg, base, bdp;
	 
	spl6();
	bdp = (umd>>18) & 0x0f;
	if (bdp) {
		ubavad.uba_dpr[bdp] |= BNE; /* purge */
		mfree(bdpmap, 1, bdp);
		if (bdpwant) {
			bdpwant = 0;
			wakeup(bdpmap);
		}
	}
	nseg = (umd>>9) & 0x1ff;
	base = (umd) & 0x1ff;
	mfree(ubmap, nseg, base);
	if (ubmwant) {
		ubmwant = 0;
		wakeup(ubmap);
	}
	spl0();
}

ubainit()
{
	register n;

	n = (((int)(&end)) - ((int)(&_sdata)) + 0x1ff) >> 9;
	ubaclr();
	mfree(ubmap, 496-n, n);
	mfree(bdpmap, 15, 1);
	ubcoffs = (int)(&_sdata) & ~0x1ff;
}

paddr_t
ubmaddr(bp, umd)
register struct buf *bp;
{
	register base, nseg;
	register bdp, pfn;
	int	count;
	extern mbautl[], Mbamap[];

	pfn = 0;
	if (bdp = ((umd>>18) & 0xf)) {
		ubavad.uba_dpr[bdp] |= BNE;
		if (paddr(bp) & 01)
			pfn |= BO;
	}
	pfn |= MRV | bdp << 21;
	nseg = (umd>>9) & 0x1ff;
	count = btoc(bp->b_bcount + (paddr(bp)&0x1ff));
	if (count<nseg)
		nseg = count;
	base = umd & 0x1ff;
	if (bp->b_flags&B_PHYS) {
		register *pt, *io;
		register pf;

		ptaccess(bp->b_proc, Mbamap, mbautl);
		io = &ubavad.uba_map[base];
		pf = paddr(bp)>>9;
		if (pf & 0x200000) {	/* I/O to stack */
			pf += ((struct user *)mbautl)->u_pcb.pcb_szpt*128 - 0x400000;
		}
		pt = mbautl + UPAGES*128 + pf;
		while (--nseg >= 0)
			if ((*io++ = ((*pt++ & 0x1ffff) | pfn)) == pfn)
				panic("uba, zero entry");
		*io = 0;
		return((base<<9)|(paddr(bp)&0x1fe));
	} else {
		return(paddr(bp) - ubcoffs);
	}
}

ubasrv(vec)
{
	if (vec==0) {
		ubazero++;
		return;
	}
	if (ubavad.uba_cnfgr&CFGFLT) {
		printf("UBA SBI fault %x %x\n", ubavad.uba_cnfgr, ubavad.uba_sr);
		return;
	}
	printf("UBA status %x, fubar %o\n", ubavad.uba_sr, ubavad.uba_fubar<<2);
	ubavad.uba_sr = ubavad.uba_sr;
	/* possible normal interrupt here ? */
}
 
ubastray(vec)
{
	printf("stray UBA interrupt at %o\n",vec);
}
ubaclr()
{
	register base, i, n;

	base = (((int)(&_sdata)) >> 9) & PG_PFNUM;
	n = (((int)(&end)) - ((int)(&_sdata)) + 0x1ff) >> 9;
	for ( i=0; i<n; i++) {
		ubavad.uba_map[i] = MRV | base++;
	}
}

