# C library -- getgid

# gid = getgid();
# returns real gid

	.set	getgid,47
.globl	_getgid

_getgid:
	.word	0x0000
	chmk	$getgid
	ret
