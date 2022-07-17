	.align	2
_fubyte:	.globl	_fubyte
	.word	0x0000
	prober	$3,$1,*4(ap)	# byte accessible ?
	beql	eret			# no
	movzbl	*4(ap),r0
	ret

	.align	2
_subyte:	.globl	_subyte
	.word	0x0000
	probew	$3,$1,*4(ap)	# byte accessible ?
	beql	eret			# no
	movb	8(ap),*4(ap)
	clrl	r0
	ret

	.align	2
_fuword:	.globl	_fuword
_fuiword:.globl	_fuiword
	.word	0x0000
	prober	$3,$4,*4(ap)
	beql	eret
	movl	*4(ap),r0
	ret

	.align	2
_suword:	.globl	_suword
_suiword:.globl	_suiword
	.word	0x0000
	probew	$3,$4,*4(ap)
	beql	eret
	movl	8(ap),*4(ap)
	clrl	r0
	ret
eret:
	mnegl	$1,r0			# error return
	ret

# Check user address: useracc(addr, count, rw)
# return 0 on no access.
	.align	2
_useracc:	.globl	_useracc
	.word	0x0000
	movl	4(ap),r0
	movl	8(ap),r1
	tstl	12(ap)
	bneq	userar
# B_WRITE
	cmpl	$512,r1
	bgeq	uaw2
uaw1:
	prober	$3,$512,(r0)
	beql	uaerr
	addl2	$512,r0
	acbl	$513,$-512,r1,uaw1
uaw2:
	prober	$3,r1,(r0)
	beql	uaerr
	movl	$1,r0
	ret

# B_READ
userar:
	cmpl	$512,r1
	bgeq	uar2
uar1:
	probew	$3,$512,(r0)
	beql	uaerr
	addl2	$512,r0
	acbl	$513,$-512,r1,uar1
uar2:
	probew	$3,r1,(r0)
	beql	uaerr
	movl	$1,r0
	ret

uaerr:
	clrl	r0
	ret
