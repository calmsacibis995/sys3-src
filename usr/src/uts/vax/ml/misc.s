_savfp:	.globl	_savfp
_restfp:	.globl	_restfp
	.word	0x0000
	ret

	.align	2
_addupc:	.globl	_addupc
	.word	0x0000
	movl	8(ap),r2	# &u.u_prof
	subl3	8(r2),4(ap),r0	# corrected pc
	blss	addret
	extzv	$1,$31,r0,r0	# logical right shift
	extzv	$1,$31,12(r2),r1	# ditto for scale
	mull2	r1,r0
	ashl	$-14,r0,r0
	incl	r0
	bicb2	$1,r0
	cmpl	r0,4(r2)	# length
	bgequ	addret
	addl2	(r2),r0		# base
	probew	$3,$2,(r0)
	beql	adderr
	addw2	12(ap),(r0)
addret:
	ret
adderr:
	clrl	12(r2)
	ret

	.align	2
_spl1:	.globl	_spl1
	.word	0x0000
	mfpr	$IPL,r0		# get IPL value
	mtpr	$2,$IPL		# disable RESCHED & AST interrupts
	ret

	.align	2
_spl4:	.globl	_spl4
	.word	0x0000
	mfpr	$IPL,r0
	mtpr	$0x14,$IPL	# disable bus level 4 interrupts
	ret

	.align	2
_spl5:	.globl	_spl5
	.word	0x0000
	mfpr	$IPL,r0
	mtpr	$0x15,$IPL	# disable bus level 5 interrupts
	ret

	.align	2
_spl6:	.globl	_spl6
_spl7:	.globl	_spl7
	.word	0x0000
	mfpr	$IPL,r0
	mtpr	$0x18,$IPL	# disable bus level 7 and clock interrupts
	ret

	.align	2
_spl0:	.globl	_spl0
	.word	0x0000
	mfpr	$IPL,r0
	mtpr	$0,$IPL
	ret

	.align	2
_splx:	.globl	_splx
	.word	0x0000
	mfpr	$IPL,r0
	mtpr	4(ap),$IPL
	ret
