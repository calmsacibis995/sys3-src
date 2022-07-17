# C library -- stty

.globl	_stty
.globl	cerror

	.set	stty,31

_stty:
	.word	0x0000
	chmk	$stty
	bcc	noerror
	jmp	cerror
noerror:
	ret
