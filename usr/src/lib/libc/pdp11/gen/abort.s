/ C library -- abort

.globl	_abort
iot	= 4
.globl	csav,cret

_abort:
	jsr	r0,csav
	iot
	clr	r0
	jmp	cret
