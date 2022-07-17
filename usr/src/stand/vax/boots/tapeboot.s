# Tape boot program to load and transfer
# to a program on a 9-tr mag tape 
# This program is the boot block on the mag tape.
	.set	RELOC,0x50000
	.set	HDRSIZ,040	# size of file header for VAX
	.set	MAGIC,0410	# file type id in header
	.set	BLKSIZ,512	# tape block size, bytes
	.set	TSIZ,4		# text size
	.set	DSIZ,8		# data size
	.set	BSIZ,12		# bss size
	.set	TENT,024	# task header entry loc
#
# MBA registers
	.set	MBA1,0x20012000	# MBA 1 device reg's
	.set	M_cfg,0	# MBA config reg
	.set	M_cr,4	# MBA control reg
	.set	M_stat,8	# MBA status reg
	.set	M_var,12	# MBA virt addr reg
	.set	M_bc,16	# MBA byte count reg
	.set	M_map,0x800	# start of MBA map reg's
	.set	MBAinit,1	# MBA init bit in MBA control reg
#
	.set	TM,MBA1+0x400	# start of TM reg's
	.set	TM_cs1,0	# TM control 1 reg
	.set	TM_ds,4	# status reg
	.set	TM_er,8	# error reg
	.set	TM_as,16	# attention summary
	.set	TM_fc,20	# frame count
	.set	TM_tc,36	# TM tape control
	.set	GO,1	# GO bit
	.set	RWND,06	# rewind, on-line
	.set	DCLR,010	# drive clear
	.set	SFWD,030	# space forward
	.set	SREV,032	# space reverse
	.set	READ,070	# read forward

	.set	ERR,040000	# composite error bit in status reg
	.set	TCHAR,01700
	.set	DRDY,0200	# TM/drive ready in status reg
	.set	TM_pd,7
	.set	TM_pe,14
	.set	rMBA,r10
	.set	rTM,r11
# system initialization
start:
	movl	$RELOC,sp
	moval	start,r6
	movc3	$end-start,(r6),(sp)
	jmp	*$RELOC+main
main:
	moval	msginit,r0
	bsbw	puts
	movab	*$MBA1,%rMBA
	movl	$MBAinit,M_cr(%rMBA)	# MBA initialize
	movab	*$TM,%rTM
	movl	$TCHAR,TM_tc(%rTM)	# drive no., etc.
	movl	$DCLR+GO,TM_cs1(%rTM)	# drive clear
	bsbw	rew
	clrl	r7
	bsbb	taper
	bsbb	taper
c1:
	bsbb	taper
	bneq	c2
	addl2	$512,r7
	brb	c1
c2:
	clrl	r0
	cmpl	$MAGIC,(r0)
	bneq	clrcor
# move start of exec image down to 0
	movq	TSIZ(r0),r1	# text size and data size
	movq	BSIZ(r0),r3	# bss size, symbol table size
	movl	TENT(r0),r5	# entry loc
	movl	r7,r6	# restore file size
movtxt:
	movb	HDRSIZ(r0),(r0)+
	sobgtr	r6,movtxt
# move data up to page boundary
	extzv	$0,$9,r1,r0		# byte-in-page of last text byte
	beql	clrcor
	subl3	r0,$512,r0		# room needed
	addl2	r2,r1			# current end+1 of data
	addl2	r1,r0			# new end+1 of data
	movl	r0,r7			# start here on bssz clearing
movdat:
	movb	-(r1),-(r0)
	sobgtr	r2,movdat
#
# clear core up to stack
clrcor:
	subl3	r7,$RELOC-4,r0	# no. bytes to clear
clrit:
	clrb	(r7)+
	sobgtr	r0,clrit
#
	calls	$0,(r5)
	brw	start	# returned from execution - start all over
taper:
	bitl	$DRDY,TM_ds(%rTM)
	beql	taper
	movl	$-BLKSIZ,M_bc(%rMBA)	# byte count
	ashl	$-9,r7,r0
	bisl3	$0x80000000,r0,M_map(%rMBA)	# MBA map entry
	clrl	M_var(%rMBA)	# MBA virt addr reg
	movl	$READ+GO,TM_cs1(%rTM)	# read forward
tmrdy:
	movl	TM_ds(%rTM),r2
	bbc	$TM_pd,r2,tmrdy	# TM & device ready ?
	bitl	$4,r2
	bneq	eot
	movl	TM_er(%rTM),r2
	bbc	$TM_pe,r2,donred	# any read errors ?
	clrl	TM_ds(%rTM)	# clear status - try to recover
	mnegl	$1,TM_fc(%rTM)	# frame count for backspace
	movl	$SREV+GO,TM_cs1(%rTM)	# space reverse
	brb	taper
donred:
	bispsw	$4
eot:
	rsb
# subr to rewind mag tape
rew:
	movl	$RWND+GO,TM_cs1(%rTM)	# rewind
	rsb

	.set	TXCS, 34
	.set	TXDB, 35

puts:
	mfpr	$TXCS,r1
	bbc	$7,r1,puts
	movzbl	(r0)+,r1
	beql	puts1
	mtpr	r1,$TXDB
	brb	puts
puts1:
	rsb
msginit:
	.byte	'U, 'N, 'I, 'X, ' , 't, 'a, 'p, 'e, ' , 'b, 'o, 'o, 't
	.byte	' , 'l, 'o, 'a, 'd, 'e, 'r, 0xa, 0xd, 0
end:
