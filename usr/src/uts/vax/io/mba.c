#include "sys/param.h"
#include "sys/buf.h"
#include "sys/systm.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/proc.h"
#include "sys/seg.h"
#include "sys/var.h"
#include "sys/page.h"
#include "sys/mba.h"

#define	MBAWCOM	0x30
#define	MBARCOM	0x38
extern struct mba mba0vad, mba1vad;
#define	GO	01

int mbaboff;

mbastart(bp, adcr, mba)
register struct buf *bp;
int *adcr;
register struct mba *mba;
{
	int vaddr, com;
	register int *io, num;
	register int *pt, i;
	int pf;
	extern mbautl[], Mbamap[];

	if ( (bp->b_flags & B_PHYS) == 0 ) {
		vaddr = (bp->b_un.b_addr - (char *)buffers) + mbaboff;
	} else {
		ptaccess(bp->b_proc, Mbamap, mbautl);	/* get to u-area & page tables */
		io = &mba->mba_map[128];
		vaddr = (128 << 9) | ((int)bp->b_un.b_addr & 0x1ff);
		pf = (int)bp->b_un.b_addr >> 9;
		num = (((int)bp->b_un.b_addr + bp->b_bcount - 1) >> 9) - pf +1;
		if ((bp -> b_flags & B_UAREA) != 0)  {	/* u-area should be part of the I/O */
			for(i=0; i<UPAGES; i++) {
			    if ((*io++ = PG_V | bp->b_proc->p_addr[i])==PG_V)
				panic("mba, zero u-page");
			}
			num -= UPAGES;
		}
		if (pf & 0x200000) {	/* I/O to stack */
			i = ((struct user *)mbautl)->u_pcb.pcb_szpt;
			pf = i*128 + pf - 0x400000;
		}
		pt = mbautl + UPAGES*128 + pf;
		while ( num-->0) {
			if ((*io++ = *pt++ | PG_V)==PG_V)
				panic("mba, zero entry");
		}
		*io = 0;
	}
	mba->mba_ireg.mba_regs.mba_sr = -1;	/* clear status (error) bits */
	mba->mba_ireg.mba_regs.mba_bcr = -bp->b_bcount;
	mba->mba_ireg.mba_regs.mba_var = vaddr;
	if (bp->b_flags & B_READ)
		com = MBARCOM | GO;
	else
		com = MBAWCOM | GO;
	if (pwr_act >= 0)
		*adcr = com;	/* start command */
}

mbainit()
{
	mbaclr();
	mbaboff = (int)buffers & 0x1ff;
}

mbaclr()
{
	register int *io0, *io1, *b, t, j;
	extern int Sysmap[];

	io0 = mba0vad.mba_map;
	io1 = mba1vad.mba_map;
	b = Sysmap + ((((int) buffers)>>9)&PG_PFNUM);
	j = v.v_buf + 1;
	do {
		t = PG_V | (*b++ & PG_PFNUM);
		*io0++ = t;
		*io1++ = t;
	} while (--j>0);
	*io0 = 0;		/* invalidate next entry */
	*io1 = 0;
}
