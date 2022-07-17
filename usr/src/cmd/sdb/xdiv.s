	.stab	'x,'d,'i,'v,'.,'f,0,0,0144,0,00,0
	.data	0
	.stab	'M,'A,'I,'N,'_,0,0,0,044,0,01,L11
	.align	2
L13:
	.long	0x0
	.align	2
L14:
	.long	0x4100
	.data	1
	.stab	'x,0,0,0,0,0,0,0,050,0,06,v.1
	.stab	'x,0,0,0,0,0,0,0,0376,0,00,4
	.lcomm	v.1,4
	.stab	'y,0,0,0,0,0,0,0,050,0,06,v.2
	.stab	'y,0,0,0,0,0,0,0,0376,0,00,4
	.lcomm	v.2,4
	.stab	'z,0,0,0,0,0,0,0,050,0,06,v.3
	.stab	'z,0,0,0,0,0,0,0,0376,0,00,4
	.lcomm	v.3,4
	.data	0
	.globl	_MAIN__
	.align	2
.text
LL1:

	.stab	0,0,0,0,0,0,0,0,0104,0,01,LL1

	.globl	_MAIN__
	.stab	'M,'A,'I,'N,'_,0,0,0,042,0,00,0

L11:
	movf	L13,v.1
LL2:

	.stab	0,0,0,0,0,0,0,0,0104,0,02,LL2

	movf	L14,v.2
LL3:

	.stab	0,0,0,0,0,0,0,0,0104,0,03,LL3

	divf3	v.1,v.2,r0
	cvtfd	r0,r0
	cvtdf	r0,v.3
LL4:

	.stab	0,0,0,0,0,0,0,0,0104,0,04,LL4

L12:
	ret
_MAIN__:
	.word	0x0
	subl2	$.F1,sp
	jbr     L11
	.set	.F1,0
.text
