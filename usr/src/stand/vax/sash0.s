# sash0 -- sash assembler assist (VAX version)

	.globl	_main
	.globl	_xcom
	.globl	_end
L0:
	.word	0
	movl	$0x50000,sp
	subl3	$L0,$_end,r0
	movc3	r0,*$0,(sp)
	jmp	*$main0

main0:
	calls	$0,_main
	calls	$0,__exit

# _exit ()
	.globl	__exit
__exit:
	.word	0
	movl	$0x50000,sp
	calls	$0,_xcom
	pushl	$_argv
	pushl	_argc
	pushl	$0x12345678
	calls	$3,*_entry
	brb	__exit+2

# movc3 (length, from, to)
	.globl	_movc3
_movc3:
	.word	0
	movc3	4(ap),*8(ap),*12(ap)
	ret

# clrseg (buf, length)
	.globl	_clrseg
_clrseg:
	.word	0
	movc5	$0,*$0,$0,8(ap),*4(ap)
	ret

	.data
	.comm	_argc,4
