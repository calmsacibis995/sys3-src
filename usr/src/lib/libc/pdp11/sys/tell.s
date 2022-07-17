/ C library -- tell

/ long tell(f) -- returns offset of file f.

.globl	_tell, cerror
.lseek = 19.

_tell:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),r0
	sys	.lseek; 0; 0; 1
	bec	1f
	mov	$-1,r1
	jmp	cerror
1:
	mov	(sp)+,r5
	rts	pc
