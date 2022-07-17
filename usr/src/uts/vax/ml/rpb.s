# Restart parameter block
#
	.text
	.align	16
	.globl	Rpbbase
Rpbbase:
	.long	Rpbbase
	.long	0
	.long	0
	.long	0
	.align	9
