# C library -- fcntl

	.set	fcntl,62
.globl	_fcntl
.globl	cerror

_fcntl:
	.word	0x0000
	chmk	$fcntl
	bcc	noerror
	jmp	cerror
noerror:
	ret
