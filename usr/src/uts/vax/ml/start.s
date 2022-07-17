# Initialization
#
#	IPL == 1F
#	MAPEN == off
#	SCBB, PCBB not set
#	SBR, SLR not set
#	ISP, KSP not set
#
	.set	MCKVEC,4	# offset into Scbbase of machine check vector
	.globl	start
start:
	.word	0x0000
	mtpr	$HIGH,$IPL	# no interrupts yet
	mtpr	$Scbbase,$SCBB	# set SCBB
	mtpr	$_Sysmap,$SBR	# set SBR
	mtpr	$Syssize,$SLR	# set SLR
	mtpr	$_Sysmap,$P0BR	# set temp P0BR
	mtpr	$Syssize,$P0LR	# set temp P0LR
	movl	$ispstend,sp	# set ISP
# initialize i/o adatpers
	movl	$1,PHYSMBA0+4	# init
	movl	$4,PHYSMBA0+4	# interrupt enable
	movl	$1,PHYSMBA1+4	# init
	movl	$4,PHYSMBA1+4	# interrupt enable
	movl	$1,PHYSUBA+4	# init
	movl	$0x78,PHYSUBA+4	# interrupt enable
#
#	Setup restart parameter block
#
	moval	Rpbbase,r6
	movl	$Rpbbase-0x80000000,RPB_BASE(r6)
	movl	$restart-0x80000000,RPB_RESTART(r6)
	clrl	RPB_CHKSUM(r6)
	moval	restart,r0
	movl	$0x1f,r1
chksum:
	addl2	(r0)+,RPB_CHKSUM(r6)
	sobgtr	r1,chksum
	clrl	RPB_FLAG(r6)

	movl	Scbbase+MCKVEC,r5	# save machine check entry
	movab	startint+ISTK,Scbbase+MCKVEC	# set new vector address
#
# will now see how much memory there really is
#	in 64kb chunks, save number of bytes in r7
#
	mtpr	$HIGH-1,$IPL	# allow machine check interrupts
	clrl	r7
startlp:
	tstl	(r7)		# this chunk really there?
	acbl	$8096*1024-1,$64*1024,r7,startlp	# loop til machine check
	brb 	startint	# full load of memory, avoid .align
	.align	2
startint:
	mtpr	$0,$SBIFS		# clear sbi fault status
	movl	r5,Scbbase+MCKVEC	# restore machine check vector
	movl	$ispstend,sp	# reset ISP

#	clear memory starting with unitialized data (kernal)
	movab	_edata,r6
	movab	_end+8096,r5	# clear uninitialized data and some slop
strtclr:
	clrq	(r6)
	acbl	r5,$8,r6,strtclr

#
#	initialize system page table
#
	movab	_etext+511,r1	# end of kernal text segment
	bbcc	$31,r1,strt1	# turn off high order bit
strt1:
	ashl	$-9,r1,r1	# last page of kernal text
	clrl	r2		# point at first kernal text page
strtlp1:
	bisl3	$PG_V|PG_KR,r2,_Sysmap[r2]	# initialize page table entry
	aoblss	r1,r2,strtlp1	# fill text entries
	movab	_end+511,r1	# end of kernal data segment
	bbcc	$31,r1,strt2	# turn off high order bit
strt2:
	ashl	$-9,r1,r1	# last page of kernal data
strtlp2:
	bisl3	$PG_V|PG_KW,r2,_Sysmap[r2]	# fill data entries
	aoblss	r1,r2,strtlp2

#	init i/o space page table entries
	movl	$PHYSUBA/512,r1		# page frame number for uba
	movab	uba_pte,r2	# page table address
	movab	15(r1),r3	# last pt entry
strtlp3:
	bisl3	$PG_V|PG_KW,r1,(r2)+	# init pt entry
	aobleq	r3,r1,strtlp3
	movl	$PHYSUMEM/512,r1
	movab	umem_pte,r2	# page table address
	movab	15(r1),r3		# limit
strtlp4:
	bisl3	$PG_V|PG_KW,r1,(r2)+
	aobleq	r3,r1,strtlp4
	movl	$PHYSMBA0/512,r1
	movab	mba0_pte,r2
	movab	15(r1),r3
strtlp5:
	bisl3	$PG_V|PG_KW,r1,(r2)+
	aobleq	r3,r1,strtlp5
	movl	$PHYSMBA1/512,r1
	movab	mba1_pte,r2
	movab	15(r1),r3
strtlp6:
	bisl3	$PG_V|PG_KW,r1,(r2)+
	aobleq	r3,r1,strtlp6

	mtpr	$1,$TBIA	# invalidate all trans buffer entries
	mtpr	$1,$MAPEN	# turn on memory mapping
	jmp 	*$startmap	# put system virtual address in pc

startmap:
# set maxmem = btoc(r7)
	ashl	$-9,r7,_maxmem
	movl	_maxmem,_physmem
	movl	_maxmem,_freemem

#
# Setup context for proc[0] == Scheduler
#
# address first page past _end
#	this will be u area for proc[0].
#	initialize u area page table entries.
#	initialize (slightly) the pcb.

	movab	_end+511,r6
	bicl2	$0x1ff,r6	# make page boundary

# set up u area page table
	bbcc	$31,r6,strt3
strt3:
	ashl	$-9,r6,r3		# r3 = btoc(r6)
	bisl3	$PG_V|PG_KW,r3,_Sysmap+u_ptoffset	# init first u pt entry
	movab	_u,r1		# point at _u area
	mtpr	r1,$TBIS
	movab	usize*512(r1),PCB_KSP(r1)	# init ksp
	mnegl	$1,PCB_ESP(r1)		# invalidate esp
	mnegl	$1,PCB_SSP(r1)		# invalidate ssp
	movl	$0x80000000,PCB_USP(r1)		# set user sp
	movab	_u+usize*512,PCB_P0BR(r1)	# p0 page table pointer
	clrl	PCB_P0LR(r1)		# size zero page table
	movb	$4,PCB_P0LR+3(r1)	# disable ast
	movab	_u+(usize+1)*512-0x800000,PCB_P1BR(r1)	# p1 page table pointer
	movl	$0x200000,PCB_P1LR(r1)		# invalid p1 p t length
	movl	$1,PCB_SZPT(r1)		# init number pages usr page table
	addl3	$usize,r3,PCB_SZPT+4(r1)	# store into u.u_ptable[0]
	movl	$1,r2
strt5:
	incl	r3
	bisl3	$PG_V|PG_KW,r3,_Sysmap+u_ptoffset[r2]
	addl2	$512,r1
	mtpr	r1,$TBIS
	aoblss	$usize,r2,strt5

	mtpr	r6,$PCBB	# first pcb

	ldpctx			# set regs, p0br, p0lr, p1br, p1lr, 
#				astlvl, ksp, and change to kernal mode
	addl2	$8,sp		# pop dummy pc, psl
	mtpr	$0,$IPL		# enable interrupts
	movab	_end+511,r0	# calculate firstaddr
	bbcc	$31,r0,strt4
strt4:
	ashl	$-9,r0,-(sp)	# convert to clicks and stack
	calls	$1,_main	# startup, fork off /etc/init
#
# proc[1] == /etc/init now running here.
# execute code at location 0, in user mode.
#
	pushl	$PSL_CURMOD|PSL_PRVMOD	# psl, user mode, ipl= 0
	pushl	$0		# pc, $location 0
	rei 			# do /etc/init
