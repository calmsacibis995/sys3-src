#	/* Assembler program to implement the following C program */
#	int
#	abs(arg)
#	int	arg;
#	{
#		return((arg < 0)? -arg: arg);
#	}

.globl	_abs
	.align	1
_abs:
	.word	0x0000
	movl	4(ap),r0
	bgeq	absl
	mnegl	r0,r0
absl:
	ret
