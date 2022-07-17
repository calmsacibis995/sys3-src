# C library -- times

	.set	times,43
.globl	_times
.globl	cerror

_times:
	.word	0x0000
	chmk	$times
	bcc	noerror
	jmp	cerror
noerror:
	ret
