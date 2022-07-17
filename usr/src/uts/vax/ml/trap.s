	.align	2
Xchme:
Xchms:
Xchmu:
	tstl	(sp)+
	rei

	.align	2
Xmacheck:
	pushr	$0x3f
	mfpr	$SBIFS,r0
	bicl2	$0x2000000,r0
	mtpr	r0,$SBIFS
	mfpr	$SBIER,r0
	bisl2	$0x70c0,r0
	mtpr	r0,$SBIER
	moval	24(sp),r0
	addl2	(r0)+,r0
	cmpl	(r0),$copymov
	bnequ	mach1
	popr	$0x3f
	addl2	(sp)+,sp
	movl	$eret,(sp)
	bicl2	$PSL_FPD,4(sp)
	rei
mach1:
	callg	24(sp),_macheck
	halt

	.align	2
Xkspinv:
	pushr	$0x3f
	pushab	KSPmsg
	calls	$1,_printf
	halt

# Massbus 0 adapter interrupts

	.align	2
Xmba0int:
	pushr	$0x3f		# save r0 - r5
	movab	_mba0vad,r0	# point at mba regs
	movl	MBA_AS(r0),r1		# get attn summary bits
	movzbl	r1,-(sp)		# push attn summary as arg
	pushl	MBA_SR(r0)	# pass sr as argument
	mnegl	$1,MBA_SR(r0)	# clear attention bit
	calls	$2,_hpintr	# call rp06 interrupt dispatcher
	brw 	int_ret		# merge with common interrupt code

# Massbus 1 adapter interrupts

	.align	2
Xmba1int:
	pushr	$0x3f
	movab	_mba1vad,r0
	pushl	MBA_AS(r0)
	mnegl	$1,MBA_AS(r0)
	pushl	MBA_SR(r0)		# pass cr as argument_
	mnegl	$1,MBA_SR(r0)	# clear attention bit
	calls	$2,_htintr	# call te16 interrupt dispatcher
	brw 	int_ret		# return from interrupt

# Unibus adapter interrupts

	.align	2
Xubaint:	#  UBA interrupts
	pushr	$0x3f
	mfpr	$IPL,r2		# get br level
	movl	_ubavad+UBA_BR4-20*4[r2],r3	# get unibus device vector
	bleq	ubasrv	 # branch if zero vector or UBA service required

ubanorm:
	movl	_UNIvec(r3),r1	# get interrupt service routine address
	beql	ubastray	# NULL denotes stray
	extzv	$27,$4,r1,-(sp)	# controller code is in bits 27-30
	bicl2	$0x78000000,r1	# clear code
	calls	$1,(r1)
	brw	int_ret

.globl	_ubasrv
ubasrv:
	pushl	r3
	calls	$1,_ubasrv
	brb	int_ret

.globl	_ubastray
ubastray:
	pushl	r3
	calls	$1,_ubastray
	brb	int_ret

# Console receiver interrupt

	.align	2
Xconrint:
	pushr	$0x3f		# save registers 0 - 5
	calls	$0,_conrint
	brb 	int_ret		# merge

# Console transmit interrupt
	.align	2
Xconxint:
	pushr	$0x3f		# save registers 0 - 5
	calls	$0,_conxint
	brb 	int_ret

# Clock interrupt
	.align	2
Xclock:
	pushr	$0x3f		# save regs 0 - 5
	pushl	4+6*4(sp)	# push psl
	pushl	4+6*4(sp)	# push pc
	calls	$2,_clock
	brb 	int_ret

# Common code for interrupts
# At this point, the interrupt stack looks like:
#
#		 ___________
#		|    r0     | :isp
#		|-----------|
#		|    ...    |
#		|-----------|
#		|    r5     |
#		|-----------|
#		|    pc     |
#		|-----------|
#		|    psl    |
#		|___________|

int_ret:
	bbssi	$0,idleflag,int_r0	# set idle escape flag (no wait instr)
int_r0:
	popr	$0x3f		# restore regs 0 - 5
	bitl	$PSL_CURMOD,4(sp)	# interrupt from user mode?
	beql	int_r1		# no, from kernal, just rei
	tstb	_runrun		# should we reschedule?
	beql	int_r1		# no, just rei
#
# If here, interrupt from user mode, and time to reschedule.
#	to do this, we set a software level 3 interrupt to
#	change to kernal mode, switch stacks,
#	and format kernal stack for a `qswitch' trap to force
#	a reschedule.
#
	mtpr	$0x18,$IPL	# make sure interrupt doesn't happen now
	mtpr	$3,$SIRR	# request level 3 software interrupt
int_r1:
	rei 			# return to interrupted process
#
# Trap and fault vector routines
#
#
#	Quiescent state (Software level 1 interrupt)
#
	.align	2
Xquiescent:
	pushr	$0x3fff
	calls	$0,_pwr
	popr	$0x3fff
	rei
#
#	Reschedule trap (Software level 3 interrupt)
#
	.align	2
Xresched:
	movpsl	-(sp)	# get ps
	bicl2	$PSL_IPL,(sp)	# lower ipl
	pushab	resc1		# push pc
	rei			# lower ipl, jump to resc1

resc1:
	pushl	$0		# dummy code
	pushl	$RESCHED	# type
	brb 	alltraps	# merge

#	privileged instruction fault
	.align	2
Xprivinflt:
	pushl	$0		# push dummy code
	pushl	$PRIVINFLT	# push type
	brb 	alltraps	# merge

#	xfc instruction fault
	.align	2
Xxfcflt:
	pushl	$0		# push dummy code value
	pushl	$XFCFLT		# push type value
	brb 	alltraps	# merge

#	reserved operand fault
	.align	2
Xresopflt:
	pushl	$0		# push dummy code value
	pushl	$RESOPFLT	# push type value
	brb 	alltraps	# merge

#	reserved addressing mode fault
	.align	2
Xresadflt:
	pushl	$0		# push dummy code value
	pushl	$RESADFLT	# push type value
	brb 	alltraps	# merge with common code

#	bpt instruction fault
	.align	2
Xbptflt:
	pushl	$0		# push dummy code value
	pushl	$BPTFLT		# push type value
	brb 	alltraps	# merge with common code

#	Compatibility mode fault
	.align	2
Xcompatflt:
	pushl	$COMPATFLT	# push type value
	brb	alltraps

#	Trace trap

	.align	2
Xtracep:
	pushl	$0		# push dummy code value
	pushl	$TRCTRAP	# push type value
	brb 	alltraps	# go do it

#	Arithmitic trap
	.align	2
Xarithtrap:
	pushl	$ARITHTRAP	# push type value
	brb 	alltraps	# merge with common code

#	Protection  fault
	.align	2
Xprotflt:
	cmpl	8(sp),$copymov
	beql	nofault
	blbs	(sp),Xsegflt	# check for pt length violation
	addl2	$4,sp		# pop fault param word 
	pushl	$PROTFLT
	brb 	alltraps

#	Segmentation fault
	.align	2
Xsegflt:
	cmpl	8(sp),$copymov
	beql	nofault
	addl2	$4,sp
	pushl	$SEGFLT
	brb 	alltraps

nofault:
	addl2	$8,sp
	movl	$eret,(sp)
	bicl2	$PSL_FPD,4(sp)
	rei

# CHMK trap (syscall trap)
#	on entry, kernal stack:
#
#		 ___________ 
#		|    code   | :ksp
#		|-----------|
#		|    pc     |
#		|-----------|
#		|    psl    |
#		|___________|
#
#	stack (parameters) at calls to _trap:
#
#		 ___________ 
#		|    ap     | :ksp
#		|-----------|
#		|    r0     |
#		|-----------|
#		|    ...    |
#		|-----------|
#		|    r13    |
#		|-----------|
#		|    usp    |
#		|-----------|
#		|    type   |
#		|-----------|
#		|    code   |
#		|-----------|
#		|    pc     |
#		|-----------|
#		|    psl    |
#		|___________|
#

	.align	2
Xsyscall:
	pushl	$SYSCALL	# push type value
alltraps:
	movq	8(sp),_u+PCB_PC	# save pc
# Prepare arguments to _trap, note that type has already been pushed
	mfpr	$USP,-(sp)	# get usp
	pushr	$0x3fff		# registers 0 - 13
	pushl	ap		# ptr to syscall parameters
#
# Call _trap with wrong number of arguments
#   so args not popped by ret
#
	calls	$1,_trap
# Restore
	popr	$0x3fff		# restore regs 0 - 13
	mtpr	(sp)+,$USP	# restore usp
	addl2	$8,sp		# pop type, code
	rei

	.align	2
_idle:	.globl	_idle
	.word	0x0000
	mtpr	$0,$IPL		# enable interrupts
waitloc:	blbc	idleflag,waitloc	# loop until interrupt
ewaitloc:	bbcci	$0,idleflag,idle1	# clear idle escape flag
idle1:
	ret
	.data
	.globl	_waitloc
	.globl	_ewaitloc
	.align	2
_waitloc:	.long	waitloc
_ewaitloc:	.long	ewaitloc
idleflag:	.long	0
	.text
# err_print
#	print message on console and die
#	message pointed to by eptr, terminated by zero byte.

err_print:
	mtpr	$HIGH,$IPL	# disable all interrupts
	mtpr	$0,$TXCS	# disable interrupts on console transmitter
eloop1:
	mfpr	$TXCS,ewk1
	bbc 	$TXCS_BRDY,ewk1,eloop1	# loop if not ready to transmit
	tstb	*eptr		# end of message?
	beql	eout		# yes, out of loop
	movzbl	*eptr,ewk1	# get byte of message
	incl	eptr
	mtpr	ewk1,$TXDB
	brb 	eloop1
eout:
	halt

	.data
eptr:	.long	0
ewk1:	.long	0
	.text
