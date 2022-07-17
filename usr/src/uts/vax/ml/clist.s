.data
.text
LL0:.align	1
.globl	_getc
.set	.R1,0xf00
.data
.text
_getc:.word	.R1
movl	4(ap),r11
calls	$0,_spl6
movl	r0,r8
tstl	(r11)
jleq	L22
decl	(r11)
movl	4(r11),r10
cvtbl	4(r10),r0
incb	4(r10)
cvtbl	6(r10)[r0],r0
movzbl	r0,r9
cmpb	4(r10),5(r10)
jneq	L25
movl	(r10),4(r11)
jneq	L24
clrl	8(r11)
L24:movl	_cfreeli,(r10)
movl	r10,_cfreeli
jbr	L25
L22:mnegl	$1,r9
L25:pushl	r8
calls	$1,_splx
movl	r9,r0
ret
.align	1
.globl	_putc
.set	.R2,0xf00
.data
.text
_putc:.word	.R2
movl	8(ap),r11
calls	$0,_spl6
movl	r0,r8
movl	8(r11),r10
jeql	L9999
cvtbl	5(r10),r0
cmpl	r0,_cfreeli+4
jneq	L31
L9999:movl	r10,r9
movl	_cfreeli,r10
jneq	L32
pushl	r8
calls	$1,_splx
mnegl	$1,r0
ret
L32:movl	(r10),_cfreeli
clrl	(r10)
clrb	5(r10)
movb	5(r10),4(r10)
tstl	r9
jneq	L33
movl	r10,4(r11)
jbr	L34
L33:movl	r10,(r9)
L34:movl	r10,8(r11)
L31:cvtbl	5(r10),r0
incb	5(r10)
cvtlb	4(ap),6(r10)[r0]
incl	(r11)
pushl	r8
calls	$1,_splx
clrl	r0
ret
.align	1
.globl	_getcf
.set	.R3,0xc00
.data
.text
_getcf:.word	.R3
calls	$0,_spl6
movl	r0,r10
movl	_cfreeli,r11
jeql	L38
movl	(r11),_cfreeli
clrl	(r11)
clrb	4(r11)
cvtlb	_cfreeli+4,5(r11)
L38:pushl	r10
calls	$1,_splx
movl	r11,r0
ret
.align	1
.globl	_putcf
.set	.R4,0xc00
.data
.text
_putcf:.word	.R4
movl	4(ap),r11
calls	$0,_spl6
movl	r0,r10
movl	_cfreeli,(r11)
movl	r11,_cfreeli
pushl	r10
calls	$1,_splx
ret
.align	1
.globl	_getcb
.set	.R5,0xe00
.data
.text
_getcb:.word	.R5
movl	4(ap),r11
calls	$0,_spl6
movl	r0,r9
movl	4(r11),r10
jeql	L46
cvtbl	5(r10),r0
cvtbl	4(r10),r1
subl2	r1,r0
subl2	r0,(r11)
movl	(r10),4(r11)
jneq	L46
clrl	8(r11)
L46:pushl	r9
calls	$1,_splx
movl	r10,r0
ret
.align	1
.globl	_putcb
.set	.R6,0xf00
.data
.text
_putcb:.word	.R6
movl	4(ap),r11
movl	8(ap),r10
calls	$0,_spl6
movl	r0,r8
movl	8(r10),r9
jneq	L52
movl	r11,4(r10)
jbr	L53
L52:movl	r11,(r9)
L53:movl	r11,8(r10)
clrl	(r11)
cvtbl	5(r11),r0
cvtbl	4(r11),r1
subl2	r1,r0
addl2	r0,(r10)
pushl	r8
calls	$1,_splx
clrl	r0
ret

