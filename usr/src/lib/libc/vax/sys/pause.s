# C library - pause

	.set	pause,29
.globl	_pause
.globl	cerror

	.align	1
_pause:
	.word	0x0000
	chmk	$pause
	bcc	noerror
	jmp	cerror
noerror:
	clrl	r0
	ret
