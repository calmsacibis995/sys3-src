# C library -- dup

#	f = dup(of [ ,nf])
#	f == -1 for error

	.set	dup,41
.globl	_dup
.globl	cerror

_dup:
	.word	0x0000
	chmk	$dup
	bcc 	noerror
	jmp 	cerror
noerror:
	ret
