# System control block
#
	.text
	.set	ISTK,1		# handle this interrupt on the interrupt stack
	.align	9
	.globl	Scbbase
Scbbase:
	.long	Xrandom + 0x000
	.long	Xmacheck + ISTK	# machine check interrupt
	.long	Xkspinv + ISTK	# kernal stack not valid
	.long	Xpowfail + ISTK	# power fail
	.long	Xprivinflt	# privileged instruction 
	.long	Xxfcflt		# xfc instruction 
	.long	Xresopflt	# reserved operand 
	.long	Xresadflt	# reserved addressing 
	.long	Xprotflt	# protection 
	.long	Xsegflt		# segmentation 
	.long	Xtracep		# trace pending
	.long	Xbptflt		# bpt instruction
	.long	Xcompatflt	# compatibility mode fault
	.long	Xarithtrap	# arithmetic trap
	.long	Xrandom + 0x038
	.long	Xrandom + 0x03c
	.long	Xsyscall	# chmk
	.long	Xchme		# chme
	.long	Xchms		# chms
	.long	Xchmu		# chmu
	.long	Xrandom + 0x050
	.long	Xrandom + 0x054
	.long	Xrandom + 0x058
	.long	Xrandom + 0x05c
	.long	Xrandom + 0x060
	.long	Xrandom + 0x064
	.long	Xrandom + 0x068
	.long	Xrandom + 0x06c
	.long	Xrandom + 0x070
	.long	Xrandom + 0x074
	.long	Xrandom + 0x078
	.long	Xrandom + 0x07c
	.long	Xrandom + 0x080
# software interrupts
	.long	Xquiescent
	.long	Xrandom + 0x088
	.long	Xresched	# reschedule nudge
	.long	Xrandom + 0x090
	.long	Xrandom + 0x094
	.long	Xrandom + 0x098
	.long	Xrandom + 0x09c
	.long	Xrandom + 0x0a0
	.long	Xrandom + 0x0a4
	.long	Xrandom + 0x0a8
	.long	Xrandom + 0x0ac
	.long	Xrandom + 0x0b0
	.long	Xrandom + 0x0b4
	.long	Xrandom + 0x0b8
	.long	Xrandom + 0x0bc
	.long	Xclock + ISTK	# clock
	.long	Xrandom + 0x0c4
	.long	Xrandom + 0x0c8
	.long	Xrandom + 0x0cc
	.long	Xrandom + 0x0d0
	.long	Xrandom + 0x0d4
	.long	Xrandom + 0x0d8
	.long	Xrandom + 0x0dc
	.long	Xrandom + 0x0e0
	.long	Xrandom + 0x0e4
	.long	Xrandom + 0x0e8
	.long	Xrandom + 0x0ec
	.long	Xrandom + 0x0f0
	.long	Xrandom + 0x0f4
	.long	Xconrint + ISTK	# console receiver 
	.long	Xconxint + ISTK	# console transmitter

#	I/O vectors

# IPL 14
	.long	Xrandom + 0x100
	.long	Xrandom + 0x104
	.long	Xrandom + 0x108
	.long	Xubaint		# UBA br4
	.long	Xrandom + 0x110
	.long	Xrandom + 0x114
	.long	Xrandom + 0x118
	.long	Xrandom + 0x11c
	.long	Xrandom + 0x120
	.long	Xrandom + 0x124
	.long	Xrandom + 0x128
	.long	Xrandom + 0x12c
	.long	Xrandom + 0x130
	.long	Xrandom + 0x134
	.long	Xrandom + 0x138
	.long	Xrandom + 0x13c

# IPL 15
	.long	Xrandom + 0x140
	.long	Xrandom + 0x144
	.long	Xrandom + 0x148
	.long	Xubaint		# UBA br5
	.long	Xrandom + 0x150
	.long	Xrandom + 0x154
	.long	Xrandom + 0x158
	.long	Xrandom + 0x15c
	.long	Xmba0int	# mass bus adapter 0
	.long	Xmba1int	# mass bus adapter 1
	.long	Xrandom + 0x168
	.long	Xrandom + 0x16c
	.long	Xrandom + 0x170
	.long	Xrandom + 0x174
	.long	Xrandom + 0x178
	.long	Xrandom + 0x17c

# IPL 16
	.long	Xrandom + 0x180
	.long	Xrandom + 0x184
	.long	Xrandom + 0x188
	.long	Xubaint		# UBA br6
	.long	Xrandom + 0x190
	.long	Xrandom + 0x194
	.long	Xrandom + 0x198
	.long	Xrandom + 0x19c
	.long	Xrandom + 0x1a0
	.long	Xrandom + 0x1a4
	.long	Xrandom + 0x1a8
	.long	Xrandom + 0x1ac
	.long	Xrandom + 0x1b0
	.long	Xrandom + 0x1b4
	.long	Xrandom + 0x1b8
	.long	Xrandom + 0x1bc

# IPL 17
	.long	Xrandom + 0x1c0
	.long	Xrandom + 0x1c4
	.long	Xrandom + 0x1c8
	.long	Xrandom + 0x1cc
	.long	Xrandom + 0x1d0
	.long	Xrandom + 0x1d4
	.long	Xrandom + 0x1d8
	.long	Xrandom + 0x1dc
	.long	Xrandom + 0x1e0
	.long	Xrandom + 0x1e4
	.long	Xrandom + 0x1e8
	.long	Xrandom + 0x1ec
	.long	Xrandom + 0x1f0
	.long	Xrandom + 0x1f4
	.long	Xrandom + 0x1f8
	.long	Xrandom + 0x1fc

	.data
	.globl	__sdata
__sdata:	# start of data
	.text
# 0x200
# Produce a core image dump on mag tape
	.globl	doadump
doadump:
	movl	sp,dumpstack	# save stack pointer
	movab	dumpstack,sp	# reinit stack
	mfpr	$PCBB,-(sp)	# save u-area pointer
	mfpr	$MAPEN,-(sp)	# save value
	mfpr	$IPL,-(sp)
	mtpr	$0,$MAPEN	# turn off memory mapping
	mtpr	$HIGH,$IPL	# disable interrupts
	pushr	$0x3fff		# save regs 0 - 13
	calls	$0,_dump
	halt

	.data
	.align	2
	.globl	dumpstack
	.space	58*4		# seperate stack for tape dumps
dumpstack: 
	.space	4
	.text
