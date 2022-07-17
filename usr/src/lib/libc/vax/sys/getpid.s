# getpid -- get process ID

	.set	getpid,20
.globl	_getpid

_getpid:
	.word	0x0000
	chmk	$getpid
	ret
