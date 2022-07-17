	.data
	.text
	.align	1
	.globl	_main
_main:
	.word	.R1
	subl2	$.F1,sp
	movl	$1,-4(fp)
	.data	1
L14:
	.byte	0x69,0x3d,0x25,0x64,0xa,0x0
	.text
	pushl	-4(fp)
	pushl	$L14
	calls	$2,_printf
	ret
	.set	.R1,0x0
	.set	.F1,4
	.data
