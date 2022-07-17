#include "sys/param.h"
#include "sys/systm.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/inode.h"
#include "sys/proc.h"
#include "sys/seg.h"
#include "sys/map.h"
#include "sys/reg.h"
#include "sys/psl.h"
#include "sys/utsname.h"
#include "sys/mtpr.h"
#include "sys/clock.h"
#include "sys/page.h"

/*
 * Machine-dependent startup code
 */
startup(firstaddr)
{
	register i, n;
	register mbase;
	extern long mempte[];

	/*
	 * Initialize maps
	 */

	printf("\nUNIX/%s: %s%s\n", utsname.release, utsname.sysname, utsname.version);
	printf("real mem  = %d\n", maxmem*ctob(1) );
	n = (maxmem + 0xfff) >> 12;
	mbase = firstaddr + USIZE;
	for (i=0; i<n; i++)
		mempte[i] = PG_V|PG_KW|mbase++;
	meminit(mbase, maxmem);
	maxmem -= mbase + 1;
	printf("avail mem = %d\n", maxmem*ctob(1));
	if(MAXMEM < maxmem)
		maxmem = MAXMEM;
	mfree(swapmap, nswap, 1);
	swplo--;
	mbainit();		/* setup mba mapping regs map */
	ubainit();		/* setup uba mapping regs map */
}

/*
 * Start clock
 */
clkstart()
{
	mtpr(NICR, -16667);	/* 16.667 milli-seconds */
	mtpr(ICCS,ICCS_RUN+ICCS_IE+ICCS_TRANS+ICCS_INT+ICCS_ERR);
}

clkset(oldtime)
time_t	oldtime;
{
	time = udiv(mfpr(TODR),100);
	while (time < oldtime)
		time += SECYR;
}

clkreld()
{
	mtpr(ICCS, ICCS_RUN + ICCS_IE + ICCS_INT + ICCS_ERR);
}

/*
 * Send an interrupt to process
 */
sendsig(p, n)
{
	register int *usp, *regs;
	register int mask, r, spa, t;
	int *s;

	regs = u.u_ar0;
	usp = (int *)regs[SP];
	grow((unsigned)(usp-20));
	mask = (fuword(p) & 0xfff) | 0x3f; /* get register save mask (save r0-r5) */
	suword( (caddr_t) --usp, n);	/* sig # as param */
	suword( (caddr_t) --usp, 1);	/* one parameters */
	s = usp;
	spa = ((int) usp) & 0x3;
	if (spa) usp = (int *)((int) (usp - 1) & ~ 0x3);
	t = 11;
	for (r=0x800; r; r>>=1) {
		if (mask & r) suword((caddr_t) --usp, regs[t]);
		t--;
	}
	suword( (caddr_t) --usp, regs[PC]);
	suword( (caddr_t) --usp, regs[FP]);
	suword( (caddr_t) --usp, regs[AP]);
	suword( (caddr_t) --usp, (spa << 30) | (0x2 << 28)
				| (mask << 16) | (regs[PS] & 0xffff));
	suword( (caddr_t) --usp, 0);
	regs[SP] = (int)usp;
	regs[FP] = (int)usp;
	regs[AP] = (int)s;
	regs[PC] = p + 2;
	regs[PS] &= ~(PS_CM|PS_FPD|PS_T|PS_N|PS_Z|PS_V|PS_C);
}

mtpr(regno, value)
{
	asm("	mtpr	8(ap),4(ap)");
}

mfpr(regno)
{
	asm("	mfpr	4(ap),r0");
}

/*
 * copy count bytes from from to to.
 */
bcopy(from, to, count)
caddr_t from, to;
{
	asm("	movc3	12(ap),*4(ap),*8(ap)");
}

/*
 * create a duplicate copy of a process
 */
procdup(p)
register struct proc *p;
{
	extern int forkutl[], Forkmap[];
	register int i, *ip, *jp, *kp;

	if (memall(Forkmap,USIZE) == 0)
		return(NULL);	/* no memory available */

	/* make u-area and page tables addressable */
	for( i=USIZE; --i>=0; ) {
		Forkmap[i] |= PG_V|PG_KW;
		mtpr(TBIS, forkutl + 128*i);
	}

	if (u.u_dsize)
		if (memall(&forkutl[UPAGES*128+u.u_tsize],u.u_dsize) == 0) {
			memfree(Forkmap,USIZE);
			return(NULL);
		}
	if (u.u_ssize)
		if (memall(&forkutl[USIZE*128-u.u_ssize],u.u_ssize) == 0)  {
			memfree(&forkutl[UPAGES*128+u.u_tsize],u.u_dsize);
			memfree(Forkmap,USIZE);
			return(NULL);
		}

	/* copy the u-area */
	bcopy(&u, forkutl, ctob(UPAGES));

	/* copy the data segment */
	ip = ((int *)&u) + UPAGES*128 + u.u_tsize;
	jp = ip + u.u_dsize;
	kp = forkutl + UPAGES*128 + u.u_tsize;
	while(ip < jp) {
		copyseg(*ip++&PG_PFNUM, *kp);
		*kp++ |= PG_V|PG_UW;
	}

	/* copy stack segment */
	jp = ((int *)&u) + USIZE*128;
	ip = jp - u.u_ssize;
	kp = forkutl + USIZE*128 - u.u_ssize;
	while(ip < jp) {
		copyseg(*ip++&PG_PFNUM, *kp);
		*kp++ |= PG_V|PG_UW;
	}

	/* copy text page table entries */
	bcopy( ((int *)&u) + UPAGES*128, forkutl + UPAGES*128, 
		sizeof(struct pt_entry)*u.u_tsize);

	/* clear unused page taple entries */
	ip = forkutl + UPAGES*128 + u.u_tsize + u.u_dsize;
	jp = forkutl + USIZE*128 - u.u_ssize;
	while(ip < jp) 
		*ip++ = 0;

	/* store page numbers of new page table in new u-area */
	for(i=u.u_pcb.pcb_szpt; --i>=0; )
		((struct user *)forkutl)->u_ptable[i] = (Forkmap+UPAGES)[i]
				& PG_PFNUM;

	/* store page numbers of new u-area in proc */
	for(i=UPAGES; --i>=0; )
		p->p_addr[i] = Forkmap[i];
	return(1);
}


/*
 * change protection codes of a segment
 */
chgprot(text, data, stack)
{
	register *ptaddr, i;

	if (text || data) {
		ptaddr = (int *)mfpr(P0BR);
		if (text)
			for(i=0; i<u.u_tsize; i++) {
				ptaddr[i] &= ~PG_PROT;	/* clear prot bits */
				ptaddr[i] |= PG_V|text;
				mtpr(TBIS, ctob(i));
			}
		if (data)
			for(i=u.u_tsize; i<u.u_tsize+u.u_dsize; i++) {
				ptaddr[i] &= ~PG_PROT;	/* clear prot bits */
				ptaddr[i] |= PG_V|data;
				mtpr(TBIS, ctob(i));
			}
		}
	if (stack) {
		ptaddr = (int *)(mfpr(P1BR) + 4 * mfpr(P1LR));
		for(i=0; i<u.u_ssize; i++) {
			ptaddr[i] &= ~PG_PROT;	/* clear prot bits */
			ptaddr[i] |= PG_V|stack;
			mtpr(TBIS, 0x80000000-ctob(u.u_ssize-i));
		}
	}
}

chksize(text, data, stack)
{
	if ((text + data + stack) > MAXUMEM
		|| text + data + stack + UPAGES + MAXUMEM/128 > maxmem) {
		u.u_error = ENOMEM;
		return(-1);
	}
	return(0);
}

/*
 * expand a page table
 */
ptexpand(change)
register int change;
{
	register int  *p1, *p2, i, movept;
	int  newpt[MAXUMEM/128];
	extern  int  Umap[];

	if (change <= 0)
		return(change);
	if (change > MAXUMEM/128 || memall(newpt,change) == 0)
		return(-1);

	/* calculate where stack entries start */
	movept = (0x200000 - mfpr(P1LR))/128;

	/* copy the stack page table pages down */
	for(i=0; i<movept; i++) {
		Umap[USIZE+change-1-i] = Umap[USIZE-1-i];
		mtpr(TBIS, ((int)&u) + 512*(USIZE+change-1-i));
	}

	/* insert and clear new pages */
	for(i=0; i<change; i++) {
		Umap[USIZE-movept+i] = newpt[i] | (PG_V|PG_KW);
		mtpr(TBIS,((int)&u) + 512*(USIZE-movept+i));
		clearseg(newpt[i]);
	}

	/* copy any stack entries to new page, then clear entry */
	p1 = (int *)mfpr(P1BR);
	p1 += mfpr(P1LR);
	p2 = p1 + 128*change;
	while ((int)p1 & 0x1ff) {
		*p2++ = *p1;
		*p1++ = 0;
	}

	/* fixup page table numbers in u-area */
	for(i=u.u_pcb.pcb_szpt+change; --i>=0; )
		u.u_ptable[i] = (Umap+UPAGES)[i] & PG_PFNUM;
	u.u_pcb.pcb_szpt += change;	/* new number of page tables */
	u.u_pcb.pcb_p1br = mfpr(P1BR) + 512*change;
	mtpr(P1BR, u.u_pcb.pcb_p1br);
	mtpr(TBIA,0);
	return(change);
}

/*
 * make u-area and page tables addressable
 */

ptaccess(p, map, addr)
register struct proc *p;
register int map[], addr[];
{
	register int i;

	/* make u-area addressable */
	for(i=UPAGES; --i>=0; ) {
		map[i] = p->p_addr[i] | (PG_V|PG_KW);
		mtpr(TBIS, addr + 128*i);
	}

	/* make page tables addressable */
	for(i=((struct user *)addr)->u_pcb.pcb_szpt; --i>=0; ) {
		(map+UPAGES)[i] = ((struct user *)addr)->u_ptable[i]
				| (PG_V|PG_KW);
		mtpr(TBIS, addr + 128*(i+UPAGES));
	}
}

char *macher[] = {
	"CP Read Timeout",
	"-",
	"CP Translation Buffer Parity Error",
	"CP Cache Parity Error",
	"-",
	"CP Read Data Substitute",
	"-",
	"-",
	"-",
	"-",
	"IB Translation Buffer Parity Error",
	"-",
	"IB Read Data Substitute",
	"IB Read Timeout",
	"-",
	"IB Cache Parity Error",

	"Control Store Parity Error",
	"CP Translation Buffer Parity Error",
	"CP Cache Parity Error",
	"CP Read Timeout",
	"CP Read Data Substitute",
	"Microcode lost"
};
macheck(type,ces,upc,vaddr,dr,tb0,tb1,paddr,par,sbi,pc,psl)
{
	printf("\nMachine check, type %x\n",type);
	if ((type&0xf0)==0)
		printf("%s Fault\n",macher[type&0xf]);
	else
		printf("%s Abort\n",macher[16+(type&0xf)]);
	printf("pc = %x, psl = %x\n",pc, psl);
	printf("virtual/physical addr = %x/%x\n",vaddr,paddr<<2);
	printf("ces = %x, sbi = %x\n",ces,sbi);
}
