/*
 * Location of the users' stored registers relative to R0.
 * Usage is u.u_ar0[XX].
 */
#ifdef vax
#define	R0	0
#define	R1	1
#define	R2	2
#define	R3	3
#define	R4	4
#define	R5	5
#define	R6	6
#define	R7	7
#define	R8	8
#define	R9	9
#define	R10	10
#define	R11	11
#define	R12	12
#define	R13	13

#define	AP	12
#define	FP	13
#define	SP	14
#define	PS	18
#define	PC	17

#else
#define	R0	(0)
#define	R1	(-2)
#define	R2	(-9)
#define	R3	(-8)
#define	R4	(-7)
#define	R5	(-6)
#define	R6	(-3)
#define	R7	(1)

#define	SP	(-3)
#define	PC	(1)
#define	PS	(2)
#endif