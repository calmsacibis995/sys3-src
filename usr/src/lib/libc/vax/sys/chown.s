# C library -- chown

# error = chown(string, owner);

	.set	chown,16
.globl	_chown
.globl	cerror

_chown:
	.word	0x0000
	chmk	$chown
	bcc 	noerror
	jmp 	cerror
noerror:
	clrl	r0
	ret
