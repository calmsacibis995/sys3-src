/*
 * Trap type values
 */

#ifdef vax
#define	RSADFLT	0	/* reserved addressing fault */
#define	PRIVFLT	1	/* privileged instruction fault */
#define	BPTFLT	2	/* bpt instruction fault */
#define	XFCFLT	3	/* xfc instruction fault */
#define	RSOPFLT	4	/* reserved operand fault */
#define	SYSCALL	5	/* chmk instruction (syscall trap) */
#define	ARTHTRP	6	/* arithmetic trap */
#define	RESCHED	7	/* software level 1 trap (reschedule trap) */
#define	SEGFLT	8	/* segmentation fault */
#define	PROTFLT	9	/* protection fault */
#define	TRCTRAP	10	/* trace trap */
#define	CMPTFLT	11	/* compatibility mode fault */
#else
#define	RSADFLT	0	/* reserved addressing fault */
#define	PRIVFLT	1	/* privileged instruction fault */
#define	BPTFLT	2	/* bpt instruction fault */
#define	IOTFLT	3	/* iot instruction fault */
#define	EMTFLT	5	/* emt instruction fault */
#define	SYSCALL	6	/* trap instruction (syscall trap) */
#define	ARTHTRP	8	/* arithmetic trap */
#define	SEGFLT	9	/* segmentation fault */
#define	PRTYFLT	10	/* memory parity fault */
#define	RESCHED	12	/* software level 1 trap (reschedule trap) */
#endif
