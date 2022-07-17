#include	"crash.h"
#include	"sys/acct.h"
#include	"sys/file.h"

#define	LSZ	(sizeof(long))
#define	PCB	x.u.u_pcb.pcb
#define	FP	(ip+2)
#define	PC	(ip+3)

struct	user	u;
union	uarea	x;

pruarea(procslot)
int	procslot;
{
	register  int  i;

	if(procslot > v.v_proc) {
		printf("%4d out of range, use Process Table Slot\n", procslot);
		return(-1);
	}
	if(getuarea(procslot) == -1) {
		error("bad read of uarea");
		return(-1);
	}


	printf("USER ID's:\t");
	printf("uid: %u, gid: %u, real uid: %u, real gid: %u\n",
		x.u.u_uid, x.u.u_gid, x.u.u_ruid, x.u.u_rgid);
	printf("PROCESS TIMES:\t");
	printf("user: %ld, sys: %ld, child user: %ld, child sys: %ld\n",
		x.u.u_utime, x.u.u_stime, x.u.u_cutime, x.u.u_cstime);
	printf("FILE I/O:\tuser addr: ");
	printf("%08ld, file offset: %ld, bytes: %u,\n\t\tsegment: %s\n",
		x.u.u_base, x.u.u_offset, x.u.u_count,
		x.u.u_segflg == 0 ? "data" :
		(x.u.u_segflg == 1 ? "sys" : "text"));
	printf("ACCOUNTING:\t");
	printf("command: %.14s, memory: %ld, type: %s%s\n\t\tstart: %s",
		procslot ? x.u.u_comm : "swapper",
		x.u.u_mem, x.u.u_acflag & AFORK ? "fork" : "exec",
		x.u.u_acflag & ASU ? ", super user" : "",ctime(&x.u.u_start));
	printf("OPEN FILES:\t");
	printf("file desc: ");
	for(i = 0; i < NOFILE; i++)
		if(x.u.u_ofile[i] != 0)
			printf(" %3d", i);
	printf("\n\t\tfile slot: ");
	for(i = 0; i < NOFILE; i++)
		if(x.u.u_ofile[i] != 0)
			printf(" %3d", ((unsigned)x.u.u_ofile[i] -
				File->n_value) / sizeof (struct file));
	printf("\n");
	printf("\nPROCESS CONTROL BLOCK:\n");
	printf("\tksp: %08lx esp: %08lx ssp: %08lx usp: %08lx\n",
		x.u.u_pcb.pcb_ksp, x.u.u_pcb.pcb_esp, x.u.u_pcb.pcb_ssp, x.u.u_pcb.pcb_usp);
	printf("\tr0:  %08lx r1:  %08lx r2:  %08lx r3:  %08lx r4:  %08lx\n",
		x.u.u_pcb.pcb_r0, x.u.u_pcb.pcb_r1, x.u.u_pcb.pcb_r2, x.u.u_pcb.pcb_r3, x.u.u_pcb.pcb_r4);
	printf("\tr5:  %08lx r6:  %08lx r7:  %08lx r8:  %08lx r9:  %08lx\n",
		x.u.u_pcb.pcb_r5, x.u.u_pcb.pcb_r6, x.u.u_pcb.pcb_r7, x.u.u_pcb.pcb_r8, x.u.u_pcb.pcb_r9);
	printf("\tr10: %08lx r11: %08lx r12: %08lx r13: %08lx\n",
		x.u.u_pcb.pcb_r10, x.u.u_pcb.pcb_r11, x.u.u_pcb.pcb_r12, x.u.u_pcb.pcb_r13);
	printf("\tpc:  %08lx psl: %08lx szpt: %d\n",
		x.u.u_pcb.pcb_pc, x.u.u_pcb.pcb_psl, x.u.u_pcb.pcb_szpt);
	printf("\tp0br: %08lx p0lr: %08lx p1br: %08lx  p1lr: %08lx\n",
		x.u.u_pcb.pcb_p0br,x.u.u_pcb.pcb_p0lr,x.u.u_pcb.pcb_p1br,x.u.u_pcb.pcb_p1lr);
}

prstack(procslot)
int	procslot;
{
	register  int	*ip;

	if(procslot > v.v_proc) {
		printf("%6lo out of range, use Process Table Slot\n", procslot);
		return(-1);
	}
	if(getuarea(procslot) == -1) {
		error("bad read of uarea");
		return(-1);
	}
	printf("KERNEL STACK:");
	for(ip=(int *)&x.u.u_stack[0]; ip<(int *)&x.stk[UPAGES*512]; ip++)
		if(*ip)	 break;
	while(((int)ip - (int)&x) % 16)
		--ip;
	while(ip < (int *) &x.stk[UPAGES * 512]) {
		if((((int)ip - (int)&x) % 16) == 0)
			printf("\n%08lx:\t",U->n_value + ((int)ip - (int)&x));
		printf("  %08lx", *ip++);
	}
	printf("\n");
}

prtrace(d,procslot)
	int	procslot;
	int	d;
{
	register  int  *ip;
	struct	nlist	*sp;

	if(procslot > v.v_proc) {
		printf("%6lo out of range, use Process Table Slot\n", procslot);
		return(-1);
	}
	if(getuarea(procslot) == -1) {
		error("bad read of uarea");
		return(-1);
	}

	printf("STACK TRACE:\n");
	ip = (int *)((x.u.u_pcb.pcb_r13 - U->n_value) + (int)&x);

	while(*ip == NULL) ++ip; /* align on psw */
	if((unsigned)*ip < 0x80040000 && (unsigned)*ip >= 0x80000000) 
		ip -= 3;  /*assume aligned on pc, backup to psw*/
	for(; ip < (int *)&x.stk[UPAGES*512]; ) {
		while(*ip == NULL) ++ip; /* align on psw */
		if((unsigned)*FP < 0x80040000) return;
		if((((unsigned)*FP & 0x80040000) != 0x80040000) &&
			(unsigned)*FP > 0x80040000) {
			printf("\tcurdled stack\n");
			return;
		}
		sp = (struct nlist *)search(*PC,
			(N_TEXT | N_EXT), (N_TEXT | N_EXT));
		if(sp == 0)
			printf("\tno match\n");
		else if(sp->n_name[0] == '_')
			printf("\t%.7s\n", &sp->n_name[1]);
		else
			printf("\t%.8s\n", sp->n_name);

		ip = (int *)((*FP - U->n_value) + (int)&x);

	}
}
