# C library - alarm

	.set	alarm,27
.globl	_alarm

	.align	1
_alarm:
	.word	0x0000
	chmk	$alarm
	ret
