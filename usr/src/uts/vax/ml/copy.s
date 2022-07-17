	.align	2
	.globl	_copyseg
_copyseg:
	.word	0x0000
	mfpr	$IPL,-(sp)
	mtpr	$HIGH,$IPL	# turn off interrupts
	bisl3	$PG_V|PG_KR,4(ap),_Sysmap+CMAP1
	bisl3	$PG_V|PG_KW,8(ap),_Sysmap+CMAP2
	mtpr	$CADDR1,$TBIS	# invalidate entry for copy 
	mtpr	$CADDR2,$TBIS
	movc3	$512,CADDR1,CADDR2
	mtpr	(sp)+,$IPL
	ret

	.align	2
	.globl	_clearseg
_clearseg:
	.word	0x0000
	mfpr	$IPL,-(sp)
	mtpr	$HIGH,$IPL	# extreme pri level
	bisl3	$PG_V|PG_KW,4(ap),_Sysmap+CMAP1
	mtpr	$CADDR1,$TBIS
	movc5	$0,(r0),$0,$512,CADDR1
	mtpr	(sp)+,$IPL
	ret

	.align	2
	.globl	_copyin
_copyin:
	.word	0x0000
	movl	12(ap),r0
	movl	4(ap),r1
	cmpl	$512,r0
	bgeq	cishort
ciloop:
	prober	$3,$512,(r1)
	beql	ceret
	addl2	$512,r1
	acbl	$513,$-512,r0,ciloop
cishort:
	prober	$3,r0,(r1)
	beql	ceret
	brb	copymov

	.align	2
	.globl	_copyout
_copyout:
	.word	0x0000
	movl	12(ap),r0
	movl	8(ap),r1
	cmpl	$512,r0
	bgeq	coshort
coloop:
	probew	$3,$512,(r1)
	beql	ceret
	addl2	$512,r1
	acbl	$513,$-512,r0,coloop
coshort:
	probew	$3,r0,(r1)
	beql	ceret
copymov:
	movc3	12(ap),*4(ap),*8(ap)
	clrl	r0
	ret
ceret:
	mnegl	$1,r0
	ret
