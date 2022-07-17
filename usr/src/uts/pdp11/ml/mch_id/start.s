.data
.globl	start, _end, _edata, _etext, _main

/ 11/45 and 11/70 startup.
/ entry is thru 0 abs.
/ since core is shuffled,
/ this code can be executed but once

start:
/ Set up 0 & 2 to catch traps and jumps to 0
	mov	$42,0		/ illegal instruction if jump
	mov	$777,2		/ trace, high pri, catch stray if trap
	mov	$PRUSER,PS

/ set KI0 to physical 0

	mov	$77406,r3
	mov	$KISA0,r0
	mov	$KISD0,r1
	clr	(r0)+
	mov	r3,(r1)+

/ set KI1-7 to eventual text resting place
/ and UI1-7

	mov	$_end+63.,r2
	ash	$-6,r2
	bic	$!1777,r2
	mov	$_etext-8192.+63.,r5
	ash	$-6,r5
	bic	$!1777,r5
	add	r2,r5

1:
	mov	r2,UISA0-KISA0(r0)
	mov	r2,(r0)+
	mov	r5,r4
	sub	r2,r4
	cmp	$200,r4
	bge	2f
	mov	$200,r4
2:
	add	r4,r2
	dec	r4
	bge	2f
	clr	r4
	br	3f
2:
	swab	r4
	bis	$6,r4
3:
	mov	r4,UISD0-KISD0(r1)
	bic	$4,r4
	mov	r4,(r1)+
	cmp	r0,$KISA0+[7*2]
	blos	1b

	mov	KISA0+[1*2],SISA0+[1*2]

/ set KD0-7 to physical
/ and SID0-7 to RW
/ and UD0-7 to initial text

	clr	r2
	mov	$_edata,r4
	ash	$-6,r4
	bic	$!1777,r4
1:
	mov	r4,UDSA0-KDSA0(r0)
	mov	r2,(r0)+
	mov	r3,SISD0-KDSD0(r1)
	mov	r3,UDSD0-KDSD0(r1)
	mov	r3,(r1)+
	add	$200,r2
	add	$200,r4
	cmp	r0,$KDSA0+[7*2]
	blos	1b

	mov	$IO,KDSA0+[7*2]
	mov	$IO,SISA0+[7*2]

/ initialization
/ get a temp (1-word) stack
/ turn on segmentation
/ copy text to I space
/ clear bss in D space

	mov	$stk+2,sp
	mov	$25,SSR3		/ 22-bit addr KU ID space sep
	bit	$20,SSR3
	beq	1f
	mov	$70.,_cputype
	bis	$3,*$MSCR		/ Disable UNIBUS traps, non-fatal traps
1:
	inc	SSR0
	mov	$_etext,r2
	mov	$_edata,r1
	bic	$!77,r1
	add	$_etext-8192.,r1
1:
	mfpd	-(r1)
	mtpi	-(r2)
	cmp	r2,$8192.
	bhi	1b

	mov	$_edata,r1
1:
	clr	(r1)+
	cmp	r1,$_end
	blo	1b

/ set KD6 to first available core

	mov	$_etext-8192.+63.,r2
	ash	$-6,r2
	bic	$!1777,r2
	add	KISA0+[1*2],r2
	mov	r2,KDSA0+[6*2]
	movb	$usize-1,KDSD0+[6*2]+1

/ set up real sp
/ clear user block

	mov	$_u+[usize*64.],sp
	mov	KDSA0+[6*2],-(sp)
	mov	$_u,r0
	mov	r0,SL
1:
	clr	(r0)+
	cmp	r0,sp
	blo	1b

/ test for floating point hardware

	setd
nofpp:

/ Take stuff above data out of address space

	mov	$_end+63.,r0
	ash	$-6,r0
	bic	$!1777,r0
	mov	$KDSD0,r1
1:
	cmp	r0,$200
	bge	3f
	dec	r0
	bge	4f
	clr	(r1)
	br	3f
4:
	movb	r0,1(r1)
3:
	tst	(r1)+
	sub	$200,r0
	cmp	r1,$KDSD0+[6*2]
	blo	1b

/ set up previous mode and call main
/ on return, enter user mode at 0R

	mov	$PRUSER,PS
	jsr	pc,_main
	mov	$170000,(sp)
	clr	-(sp)
	rtt
