#include "sys/param.h"
#include "sys/systm.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/proc.h"
#include "sys/reg.h"
#include "sys/psl.h"
#include "sys/trap.h"
#include "sys/seg.h"
#include "sys/sysinfo.h"

#define	USER	040		/* user-mode flag added to type */
#define	NSYSENT	64

/*
 * Called from the trap handler when a processor trap occurs.
 */
trap(params, r0, r1, r2, r3, r4, r5 ,r6, r7, r8, r9, r10,
	r11, r12, r13, sp, type, code, pc, ps)
int * params;
{
	register i;
	time_t syst;

	syst = u.u_stime;
	u.u_ar0 = &r0;
	if (USERMODE(ps))
		type |= USER;
	switch (type) {

	/*
	 * Trap not expected.
	 * Usually a kernel mode bus error.
	 */
	default:
		printf("user = ");
		for(i=0; i<UPAGES; i++)
			printf("%x ", u.u_procp->p_addr[i]);
		printf("\n");
		printf("ps = %x\n", ps);
		printf("pc = %x\n", pc);
		printf("trap type %x\n", type);
		printf("code = %x\n", code);
		panic("trap");

	case PROTFLT + USER:	/* protection fault */
		i = SIGBUS;
		break;

	case PRIVFLT + USER:	/* privileged instruction fault */
	case RSADFLT + USER:	/* reserved addressing fault */
	case RSOPFLT + USER:	/* resereved operand fault */
		i = SIGILL;
		break;

	case SYSCALL + USER:	/* sys call */
	{
		register *a;
		register struct sysent *callp;

		sysinfo.syscall++;
		u.u_error = 0;
		ps &= ~PS_C;
		a = params;
		a++;		/* skip word with param count */
		i = code&0377;
		if (i >= NSYSENT)
			i = 0;
		else if (i==0) {	/* indirect */
			i = fuword(a++)&0377;
			if (i >= NSYSENT)
				i = 0;
		}
		callp = &sysent[i];
		for(i=0; i<callp->sy_narg; i++) {
			u.u_arg[i] = fuword(a++);
		}
		u.u_dirp = (caddr_t)u.u_arg[0];
		u.u_rval1 = 0;
		u.u_rval2 = u.u_ar0[R1];
		u.u_ap = u.u_arg;
		if (setjmp(u.u_qsav)) {
			if (u.u_error==0)
				u.u_error = EINTR;
		} else {
			(*callp->sy_call)();
		}
		if (u.u_error) {
			u.u_ar0[R0] = u.u_error;
			ps |= PS_C;	/* carry bit */
			if (++u.u_errcnt > 16) {
				u.u_errcnt = 0;
				runrun++;
			}
		} else {
			u.u_ar0[R0] = u.u_rval1;
			u.u_ar0[R1] = u.u_rval2;
		}
	}
	{
		register struct proc *pp;

		pp = u.u_procp;
		pp->p_pri = (pp->p_cpu>>1) + PUSER + pp->p_nice - NZERO;
		curpri = pp->p_pri;
		if (runrun == 0)
			goto out;
	}

	case RESCHED + USER:	/* Allow process switch */
		sysinfo.preempt++;
		qswtch();
		goto out;

	case ARTHTRP + USER:
		i = SIGFPE;
		break;

	/*
	 * If the user SP is below the stack segment,
	 * grow the stack automatically.
	 */
	case SEGFLT + USER:	/* segmentation exception */
		if(grow(u.u_ar0[SP]) || grow(code))
			goto out;
		i = SIGSEGV;
		break;

	case BPTFLT + USER:	/* bpt instruction fault */
	case TRCTRAP + USER:	/* trace trap */
		ps &= ~PS_T;	/* turn off trace bit */
		i = SIGTRAP;
		break;

	case XFCFLT + USER:	/* xfc instruction fault */
		i = SIGEMT;
		break;

	case CMPTFLT + USER:	/* compatibility mode fault */
				/* so far, just send a SIGILL signal */
		i = SIGILL;
		break;

	}
	psignal(u.u_procp, i);

out:
	if(issig())
		psig();
	if(u.u_prof.pr_scale)
		addupc((caddr_t)u.u_ar0[PC], &u.u_prof, (int)(u.u_stime-syst));
}

/*
 * nonexistent system call-- signal bad system call.
 */
nosys()
{
	psignal(u.u_procp, SIGSYS);
}

/*
 * Ignored system call
 */
nullsys()
{
}

stray(addr)
{
	logstray(addr);
	printf("stray interrupt at %x\n", addr);
}
