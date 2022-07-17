# define user area virtual address
	.set	kernsize,512	 # number pte's allocated to kernal
	.globl	_u
	.set	usize,4		# size of user area, in pages
	.set	maxu,64		# size of user pte area
	.set	_u,0x80000000 + kernsize*512
	.globl	_temputl
	.set	_temputl,_u+maxu*512
	.globl	_ubavad
	.set	_ubavad,_temputl+16*512
	.globl	_umemvad
	.set	_umemvad,_ubavad+16*512
	.globl	_mba0vad
	.set	_mba0vad,_umemvad+16*512
	.globl	_mba1vad
	.set	_mba1vad,_mba0vad+16*512
	.globl	_mba2vad
	.set	_mba2vad,_mba1vad+16*512
	.globl	_mba3vad
	.set	_mba3vad,_mba2vad+16*512
	.globl	_memmap
	.set	_memmap,_mba3vad+16*512
	.globl	_mbautl
	.set	_mbautl,_memmap+16*512
	.globl	_swaputl
	.set	_swaputl,_mbautl+maxu*512
	.globl	_swap2utl
	.set	_swap2utl,_swaputl+maxu*512
	.globl	_forkutl
	.set	_forkutl,_swap2utl+maxu*512
	.globl	_xswaputl
	.set	_xswaputl,_forkutl+maxu*512
	.globl	_xallutl
	.set	_xallutl,_xswaputl+maxu*512
	.globl	_xccdutl
	.set	_xccdutl,_xallutl+maxu*512
	.globl	_xswap2utl
	.set	_xswap2utl,_xccdutl+maxu*512

	.set	u_ptoffset,kernsize*4	# offset of pte's of _u
	.set	CMAP1,u_ptoffset+maxu*4	# offset of 1st seg copy entry
	.set	CMAP2,CMAP1+4		# ditto second entry
	.set	CADDR1,_u+maxu*512	# virtual addr of 1st copy segment
	.set	CADDR2,CADDR1+512	# ditto second segment
	.set	PHYSUBA,0x20006000	# real addr of uba
	.set	PHYSMBA0,0x20010000	# real addr of mba 0
	.set	PHYSMBA1,0x20012000	# real addr of mba1
	.set	PHYSUMEM,0x2013e000	# real addr of unibus memory

#
# Error messages
#
	.data
# Bootstrap program executed in user mode
# to bring up the system.
	.globl	_icode
	.globl	_szicode
	.set	exec, 11
	.align	2
_icode:
ic0:
	pushab	ic1
	pushab	ic2
	pushl	$2
	movl	sp,ap
	chmk	$exec
ic00:
	brb	ic00
	.align	2
ic1:
	.long	ic2-ic0
	.long	0
ic2:
	.byte	'/,'e,'t,'c,'/,'i,'n,'i,'t,0
	.align	2
ic9:
_szicode:
	.long	ic9-_icode

KSPmsg:
	.byte	'K,'e,'r,'n,'a,'l,' ,'s,'t,'a,'c,'k,012,0

	.align	2
	.globl	_cputype
_cputype:
	.word	780

	.align	2
	.globl	ispstack
ispstack:
	.space	512*2
ispstend:
	.space	4
 
#
#	_Sysmap:
#		system page table
#
#	structure:
#		kernsize page table entries
#			reserved for kernal text and data.
#		additional page table entries
#			used in mapping the u area (maxu entries),
#			utility entries (16 entries),
#			unibus adapter (16 entries),
#			unibus device memory (16 entries),
#			massbus adapter 0 (16 entries),
#			massbus adapter 1 (16 entries),
#			massbus adapter 2 (16 entries),
#			massbus adapter 3 (16 entries).
#			used for phys & swap I/O

	.align	2
	.globl	_Sysmap
	.globl	_Umap
	.globl	_Tempmap
	.globl	_Mbamap
	.globl	_Swapmap
	.globl	_Swap2map
	.globl	_Forkmap
	.globl	_Xswapmap
	.globl	_Xallmap
	.globl	_Xccdmap
	.globl	_Xswap2map
	.globl	_mempte
_Sysmap:
	.space	kernsize*4	# page table entries for kernal
_Umap:
	.space	maxu*4		# u-area
_Tempmap:
	.space	16*4		# utility area
uba_pte:
	.space	16*4		# unibus
umem_pte:
	.space	16*4		# unibus IO page
mba0_pte:
	.space	16*4
mba1_pte:
	.space	16*4
mba2_pte:
	.space	16*4
mba3_pte:
	.space	16*4
_mempte:
	.space	16*4		# memmap area
_Mbamap:
	.space	maxu*4		# map for phys & swap I/O
_Swapmap:
	.space	maxu*4		# swap utility area
_Swap2map:
	.space	maxu*4		# second swap area
_Forkmap:
	.space	maxu*4		# fork area
_Xswapmap:
	.space	maxu*4		# xswap area
_Xallmap:
	.space	maxu*4		# xalloc area
_Xccdmap:
	.space	maxu*4		# xccdec area
_Xswap2map:
	.space	maxu*4		# xswap2 area

Endsys:
	.set	Syssize,(Endsys-_Sysmap)/4	# number pte's in sys page table
	.globl	_mmap
	.set	_mmap,_Sysmap+CMAP2+4
	.globl	_vmmap
	.set	_vmmap,CADDR2+512
	.globl	_pmapte
	.set	_pmapte,_mmap+4
	.globl	_pmavad
	.set	_pmavad,_vmmap+512
