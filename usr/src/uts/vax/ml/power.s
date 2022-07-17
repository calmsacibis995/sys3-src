# Power Fail Interrup Handler

	.align	2
Xpowfail:
	tstl	pftime
	bneq	pfwait
	pushr	$0x3fff
#
# Save registers in restart parameter block
#
	moval	Rpbbase,r5
	movl	_Sysmap,r4	 	#this code assumes rpb at physical 0
	movl	$PG_V|PG_KW,_Sysmap
	mfpr	$PCBB,RPB_PCBB(r5)
	mfpr	$SCBB,RPB_SCBB(r5)
	mfpr	$SBR,RPB_SBR(r5)
	mfpr	$SISR,RPB_SISR(r5)
	mfpr	$SLR,RPB_SLR(r5)
	mfpr	$TODR,pftime
#
# Save all other volatile processor registers on the current stack (ISP)
#
	mfpr	$KSP,-(sp)
	mfpr	$ESP,-(sp)
	mfpr	$SSP,-(sp)
	mfpr	$USP,-(sp)
	mfpr	$ASTLVL,-(sp)
	mfpr	$PMR,-(sp)
	mfpr	$P0BR,-(sp)
	mfpr	$P0LR,-(sp)
	mfpr	$P1BR,-(sp)
	mfpr	$P1LR,-(sp)
	movl	sp,RPB_ISP(r5)
	movl	r4,_Sysmap
	movl	$-1,_pwr_act
	incl	_pwr_cnt
pfwait:
	brb	pfwait
#
# restore state and restart after power on
#
	.set POWERUP,3
	.align	9
	.globl	_clkset
restart:
	cmpl	ap,$POWERUP
	beql	powerup
	halt
powerup:
	mfpr	$TXCS,r6
	bbc	$TXCS_BRDY,r6,restart
	mtpr	$0xf03,$TXDB
restart1:
	mfpr	$TXCS,r6
	bbc	$TXCS_BRDY,r6,restart1
	movab	-512(sp),r5
	bicl2	$1,RPB_FLAG(r5)
	mtpr	RPB_SBR(r5),$SBR
	mtpr	RPB_SLR(r5),$SLR
	mtpr	RPB_SISR(r5),$SISR
	mtpr	RPB_SCBB(r5),$SCBB
	mtpr	RPB_PCBB(r5),$PCBB
	mtpr	$_Sysmap,$P0BR
	mtpr	$Syssize,$P0LR
	movl	RPB_ISP(r5),r6
	movl	$1,PHYSMBA0+4
	movl	$4,PHYSMBA0+4
	movl	$1,PHYSMBA1+4
	movl	$4,PHYSMBA1+4
	movl	$1,PHYSUBA+4
	movl	$0x78,PHYSUBA+4
	mtpr	$1,$TBIA
	mtpr	$1,$MAPEN
	jmp	*$restmap
restmap:
	movl	r6,sp
	mtpr	(r6)+,$P1LR
	mtpr	(r6)+,$P1BR
	mtpr	(r6)+,$P0LR
	mtpr	(r6)+,$P0BR
	mtpr	(r6)+,$PMR
	mtpr	(r6)+,$ASTLVL
	mtpr	(r6)+,$USP
	mtpr	(r6)+,$SSP
	mtpr	(r6)+,$ESP
	mtpr	(r6)+,$KSP
	pushl	r6
	mtpr	$0,$SBIFS
	calls	$0,_clkstart
	movl	_time,-(sp)
	calls	$1,_clkset
	movl	(sp)+,sp
	clrl	pftime
	popr	$0x3fff
	mtpr	$1,$SIRR
	rei
	.data
	.align 2
	.globl	pftime
pftime:	.space	1*4

	.globl	_pwr_act
_pwr_act:	.space	1*4

	.globl	_pwr_cnt
_pwr_cnt:	.space	1*4

.text
