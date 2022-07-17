	.stab	'z,'e,'n,'t,'r,'y,'.,'f,0144,0,00,0
	.stab	0,0,0,0,0,0,0,0,0144,0,00,0
	.data	0
	.stab	'M,'A,'I,'N,'_,0,0,0,044,0,01,L11
	.stab	's,'a,'m,0,0,0,0,0,0244,0,01,L11
	.align	2
L13:
	.long	0x41C0
	.align	2
L14:
	.long	0x41E0
	.align	2
L15:
	.long	0x4200
	.align	2
L16:
	.long	0x4210
	.data	1
	.data	0
	.stab	'j,'o,'e,0,0,0,0,0,044,0,05,L19
	.stab	'j,'i,'m,0,0,0,0,0,0244,0,07,L21
	.data	1
	.stab	'j,0,0,0,0,0,0,0,050,0,04,v.2
	.stab	'j,0,0,0,0,0,0,0,0376,0,00,4
	.lcomm	v.2,4
	.stab	'k,0,0,0,0,0,0,0,050,0,04,v.1
	.stab	'k,0,0,0,0,0,0,0,0376,0,00,4
	.lcomm	v.1,4
	.stab	'x,0,0,0,0,0,0,0,0240,0,026,4
	.stab	'y,0,0,0,0,0,0,0,0240,0,026,8
	.data	0
	.globl	_sam_
	.globl	_MAIN__
	.globl	_joe_
	.globl	_jim_
	.globl	_abort_
	.align	2
.text
LL1:

	.stab	0,0,0,0,0,0,0,0,0104,0,01,LL1

	.globl	_MAIN__
	.stab	'M,'A,'I,'N,'_,0,0,0,042,0,00,0

LL2:

	.stab	0,0,0,0,0,0,0,0,0104,0,02,LL2

L11:
	pushl	$L14
	pushl	$L13
	calls	$2,_joe_
LL3:

	.stab	0,0,0,0,0,0,0,0,0104,0,03,LL3

	pushl	$L16
	pushl	$L15
	calls	$2,_jim_
LL4:

	.stab	0,0,0,0,0,0,0,0,0104,0,04,LL4

L12:
	ret
_MAIN__:
_sam_:
	.word	0x0
	subl2	$.F1,sp
	jbr     L11
	.set	.F1,0
.text
LL5:

	.stab	0,0,0,0,0,0,0,0,0104,0,05,LL5

	.globl	_joe_
	.stab	'j,'o,'e,0,0,0,0,0,042,0,00,0

LL6:

	.stab	0,0,0,0,0,0,0,0,0104,0,06,LL6

L19:
	movl	$2,v.1
LL7:

	.stab	0,0,0,0,0,0,0,0,0104,0,07,LL7

L21:
LL8:

	.stab	0,0,0,0,0,0,0,0,0104,0,010,LL8

	movl	$3,v.2
LL9:

	.stab	0,0,0,0,0,0,0,0,0104,0,011,LL9

	calls	$0,_abort_
LL10:

	.stab	0,0,0,0,0,0,0,0,0104,0,012,LL10

L18:
L17:
	ret
_joe_:
	.word	0x0
	subl2	$.F2,sp
	movl	4(ap),-8(fp)
	movl	8(ap),-4(fp)
	addl3	$-12,fp,ap
	movl	$2,(ap)

	jbr     L19
_jim_:
	.word	0x0
	subl2	$.F2,sp
	movl	4(ap),-4(fp)
	movl	8(ap),-8(fp)
	addl3	$-12,fp,ap
	movl	$2,(ap)

	jbr     L21
	.set	.F2,12
.text
