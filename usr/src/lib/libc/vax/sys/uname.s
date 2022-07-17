# C library -- uname

	.set	utssys,57
	.set	uname,0

.globl	_uname
.globl	cerror

_uname:
	.word	0x0000
	pushl	$uname
	pushl	$0
	pushl	4(ap)
	calls	$3,sys
	ret

sys:
	.word	0x0000
	chmk	$utssys
	bcc	noerror
	jmp	cerror
noerror:
	clrl	r0
	ret
