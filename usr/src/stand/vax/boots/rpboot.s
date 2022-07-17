# RP06 disk boot program
	.set	BLKSIZ,512	# disc block size
	.set	RELOC,0x50000
	.set	HDRSIZ,040	# size of file header for VAX
	.set	MAGIC,0410	# file type id in header
	.set	TEXT,04
	.set	ENTRY,024
	.set	INOSIZ,64	# no. bytes/inode entry
	.set	INOBLK,BLKSIZ/INOSIZ	# no. inodes/disc block
	.set	INOMSK,0xfffffff8	# changes with inode size
	.set	NAMSIZ,14	# no. bytes in name field of dir entry
	.set	ENTADR,024	# offset to entry addr in task header
	.set	DIRSIZ,16	# size of directory entry, bytes
	.set	ROOTINO,2	# root dir inode no.
# MBA registers
	.set	MBA0,0x20010000	# MBA 0 device reg's
	.set	M_cr,MBA0+4	# MBA control reg
	.set	M_var,MBA0+12	# MBA virt addr reg
	.set	M_bc,MBA0+16	# MBA byte count reg
	.set	M_map,MBA0+0x800	# start of MBA map reg's
# RP06 registers
	.set	RP6TRK,19	# no. tracks/cylinder
	.set	RP6SEC,22	# no. sectors/track
	.set	RP6ST,RP6TRK*RP6SEC	# sectors/cylinder
	.set	RP,MBA0+0x400	# start of RP06 drive 0 reg's
	.set	RP_cr,RP+0	# RP06 control reg, drive 0
	.set	RP_sr,RP+4	# status reg
	.set	RP_stk,RP+0x14	# desired track/sector reg
	.set	RP_off,RP+0x24	# RP offset reg
	.set	RP_cyl,RP+0x28	# desired cyl reg
# RP06 function codes, status bits 
	.set	RP_RED,071	# read data
	.set	RP_DC,011	# drive clear function code
	.set	RP_RIP,021	# Read-In Preset function code
	.set	RP_FMT,0x1000	# format bit for offset reg
	.set	RP_DRY,0200	# drive ready, staus reg
	.set	RP_ERR,040000	# composite error, staus reg
	.set	RP_pDRY,7	# bit position of RP_DRY
	.set	RP_pERR,14	# bit position of RP_ERR

	.globl	_rpboot
_rpboot:
start:
	.word	0
	movl	$RP_RIP,*$RP_cr
	movl	$RP_FMT,*$RP_off
#	move boot image up to higher core
move:
	movl	$RELOC,sp
	moval	start,r6
	movc3	$end-start,(r6),(sp)
	jmp	*$RELOC+main-start
main:
# search inodes for pathname specified in 'names'
	moval	names,r6
	movzbl	$ROOTINO,r0
nxti:
	bsbw	iget
	clrl	bno
	clrl	bufptr
	tstb	(r6)
	beql	getfil
get1b:
	bsbw	rmblk
	beql	main	# file not found
	clrl	r7
nxtent:
	tstw	(r7)	# null dir entry
	beql	dirchk
	cmpc3	$NAMSIZ,(r6),2(r7)
	bneq	dirchk
# found component in a dir entry
	movzwl	(r7),r0	
	addl2	$NAMSIZ,r6
	brb	nxti
dirchk:	
	acbl	$BLKSIZ-1,$DIRSIZ,r7,nxtent
	brb	get1b	# get another dir block into buffer
# here if inode for desired file has been read in
getfil:
	clrl	bufptr	# new buffer ptr is low core
getlop:
	bsbw	rmblk	# get a block in the file
	beql	clear	# branch if no more file blocks to read
	addl2	$BLKSIZ,bufptr	# next page in low core
	brb	getlop
# clear core
clear:
	movl	size,r3
	movl	r3,r0
clrcor:
	clrq	(r0)+
	cmpl	r0,sp
	blssu	clrcor
# go execute file
	clrl	r0
	clrl	r5
	cmpl	(r0),$MAGIC
	bneq	exec
	movq	TEXT(r0),r1
	movl	ENTRY(r0),r5
cplp:
	movq	HDRSIZ(r0),(r0)+
	cmpl	r0,r3
	blssu	cplp
	extzv	$0,$9,r1,r0
	beql	exec
	subl3	r0,$512,r0
	addl3	r1,r2,r4
	addl2	r4,r0
dcpl:
	movq	-(r4),-(r0)
	cmpl	r1,r4
	blssu	dcpl
exec:
	calls	$0,(r5)
	brw	main
#
# subroutine to get the inode whose no. is in r0
iget:
	addl3	$(INOBLK*2)-1,r0,r8
	divl3	$INOBLK,r8,r4	# calculate inode block , put in r4
	moval	inode,bufptr
	bicl2	$INOMSK,r8
	mull2	$INOSIZ,r8
	subl2	r8,bufptr
	bsbb	rblk
	moval	faddr,r0
	moval	addr,r1
	movl	$13,r2
iget1:
	extzv	$0,$24,(r0),(r1)+
	addl2	$3,r0
	sobgtr	r2,iget1
	movl	$10,bno
	moval	addr+40,bufptr
# subr to read in 1 of blocks (bno) in 'addr[]'
rmblk:
	movl	bno,r0
	movl	addr[r0],r4
	bneq	rmblk1
	bispsw	$2
	rsb
rmblk1:
	incl	bno
# subr to read disc block no. specified in r4 from RP06, drive 0
rblk:
	clrl	r5
	ediv	$RP6ST,r4,*$RP_cyl,r1
# (r4 = block no.)/(no. blocks per cyl) -> cyl no.
# r1 = remainder = block offset with in cylinder
	clrl	r2
	ediv	$RP6SEC,r1,r1,r0
# r1 = track no., sector no. in low part RP_stk
	insv	r1,$8,$5,r0	# move track no. into RP_stk
	movl	r0,*$RP_stk	# track-sector
	movl	$-BLKSIZ,*$M_bc
	ashl	$-9,bufptr,r0	# start page no. of buffer
	addl3	$0x80000000,r0,*$M_map
	addl3	$0x80000001,r0,*$M_map+4
	bicl3	$~0x1ff,bufptr,*$M_var
	movl	$RP_RED,*$RP_cr
rprdy:
	movl	*$RP_sr,r0
	bbc	$RP_pDRY,r0,rprdy	# loop unitl ready
	bbs	$RP_pERR,r0,rperr	# branch if error
	bicpsw	$2	# set zero cc for caller success
	rsb
rperr:	# disc i/o error
	halt		# halt on error
#
bufptr: .long 0
names:
	.byte	's,'t,'a,'n,'d,0,0,0,0,0,0,0,0,0
	.byte	's,'a,'s,'h,0,0,0,0,0,0,0,0,0,0
	.byte 0
#
end:
	.set	inode,start-1024
	.set	mode,inode
	.set	nlink,mode+2
	.set	uid,nlink+2
	.set	gid,uid+2
	.set	size,gid+2
	.set	faddr,size+4
	.set	time,faddr+40
	.set	addr,time+12
	.set	bno,addr+40+512
