/ C library -- getgid

/ gid = getgid();

.globl	_getgid
.getgid = 47.

_getgid:
	mov	r5,-(sp)
	mov	sp,r5
	sys	.getgid
	mov	(sp)+,r5
	rts	pc
