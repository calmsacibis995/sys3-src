# C library -- utime
 
#  error = utime(string,timev);
 
	.set	utime,30
.globl	_utime
.globl	cerror
 
_utime:
	.word	0x0000
	chmk	$utime
	bcc	noerror
	jmp	cerror
noerror:
	ret
