# save regs and ret loc in save area - return 0
	.align	2
	.globl	_save
_save :		# save(save_area)
	.word	0x0
	mtpr	$HIGH,$IPL
	movl	4(ap),r0	# save area addr
	movab	2*4(ap),sp	# restore stack to val before 'save' call
	movl	8(fp),ap	# restore ap	"	"	"
	movl	16(fp),r1	# restore pc	"	"	"
	movl	12(fp),fp	# restore fp	"	"	"
	movq	r6,(r0)+
	movq	r8,(r0)+
	movq	r10,(r0)+
	movq	ap,(r0)+	# ap & fp
	movl	sp,(r0)+
	movl	r1,(r0)+	# ret loc of call to 'save'
	movpsl	-(sp)
	pushl	r1
	svpctx		# save reg's -> PCB
	movpsl	-(sp)	# set up for return
	bicl2	$PSL_IS|PSL_IPL,(sp)	# undo SVPCTX
	pushl	r1	# ret loc
	clrl	r0
	rei

# switch to another process's '_u' area - return 1
	.align	2
	.globl	_resume
_resume :		# resume(proc_addr,save_addr)
	.word	0x0
	mtpr	$HIGH,$IPL	# inhibit interrupts
	movl	8(ap),retloc
# map u-area
	clrl	r0
	movab	_u,r1
	movl	4(ap),r2
res1:	cvtwl	(r2)[r0],r3	# get u click number from proc
	bisl3	$PG_V|PG_KW,r3,_Umap[r0]
	mtpr	r1,$TBIS
	addl2	$512,r1
	aoblss	$usize,r0,res1
# map user page tables
	clrl	r0
	movl	_u+PCB_SZPT,r3
	jeql	res3		# no user page tables
res2:	bisl3	$PG_V|PG_KW,_u+PCB_SZPT+4[r0],_Umap+4*usize[r0]
	mtpr	r1,$TBIS
	addl2	$512,r1
	aoblss	r3,r0,res2
res3:
	movl	_u,sp		# KSP from u-area
	ashl	$9,_Umap,r5	# pcb address
	mtpr	r5,$PCBB
	ldpctx
	addl2	$8,sp		# clear ps,pc from stack
	movl	retloc,r1	# 'ssav' or 'qsav' addr
	movq	(r1)+,r6
	movq	(r1)+,r8
	movq	(r1)+,r10
	movq	(r1)+,ap
	movl	(r1)+,sp
	movl	$1,r0
	mtpr	$0,$IPL
	jmp	*(r1)+	# return to caller at 'save' address

	.data
	.align	2
retloc:	.space	1*4

	.text
	.align	2
	.globl	_setjmp
_setjmp:
	.word	0x0000
	movl	4(ap),r0
	movq	r6,(r0)+
	movq	r8,(r0)+
	movq	r10,(r0)+
	movq	8(fp),(r0)+
	movab	8(ap),(r0)+
	movl	16(fp),(r0)
	clrl	r0
	ret

	.align	2
	.globl	_longjmp
_longjmp:
	.word	0x0000
	movq	4(ap),r1
	movq	(r1)+,r6
	movq	(r1)+,r8
	movq	(r1)+,r10
	movq	(r1)+,ap
	movl	(r1)+,sp
	movl	r2,r0
	bneq	lj1
	incl	r0
lj1:
	jmp	*(r1)
