/	C library s/getpgrp

.globl	_setpgrp, _getpgrp
.setpgrp =	39.

_setpgrp:
	mov	$1,r0
	jbr	1f
	
_getpgrp:
	clr	r0
	
1:
	mov	r5,-(sp)
	mov	sp,r5
	sys	.setpgrp
	mov	(sp)+,r5
	rts	pc
