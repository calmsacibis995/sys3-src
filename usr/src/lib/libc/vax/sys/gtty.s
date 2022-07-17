# C library -- gtty

	.set	gtty,32
.globl	_gtty
.globl	cerror

_gtty:
	.word	0x0000
	chmk	$gtty
	bcc	noerror
	jmp	cerror
noerror:
	ret
