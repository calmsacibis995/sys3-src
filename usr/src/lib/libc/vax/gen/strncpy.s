# Fast assembler language version of the following C-program
#			strncpy
# which represents the `standard' for the C-library.

# Copy s2 to s1, truncating or null-padding to always copy n bytes.
# Return s1

#	char *
#	strncpy(s1, s2, n)
#	register char *s1, *s2;
#	{
#		register i;
#		register char *os1;
#	
#		os1 = s1;
#		for (i = 0; i < n; i++)
#			if ((*s1++ = *s2++) == '\0') {
#				while (++i < n)
#					*s1++ = '\0';
#				return(os1);
#			}
#		return(os1);
#	}
	.globl	_strncpy
	.text
	.align	1
_strncpy:
	.word	0x03c0		# uses regs. 0,1,2,3,4,5,6,7,8,9
	movl	$65535,r9	# size of BIG string
	movq	4(ap),r6	# dst. in r6, src. in r7
	movl	r7,r1		# working copy of src ptr for locc
L0:
	locc	$0,r9,(r1)	# look for terminal null in src.
	beql	L0		# none? look in next 65535 block
	subl3	r7,r1,r0	# length of src (almost)
	incl	r0		# add 1 to account for the null
	movl	12(ap),r2	# destination length (arg `n')
	bleq	L5		# n <= 0? if so, return dest.
	movl	r6,r3		# working copy of dest. for the movc
	movl	r7,r1		# ditto for src.
L1:
	subl3	r9,r0,r7	# r7 will be < 0 if src.ln < 65535
	bleq	L2
	movl	r9,r0		# else make iteration with length 65535
L2:
	subl3	r9,r2,r8	# r8 will be < 0 if dst.ln < 65535
	bleq	L3
	movl	r9,r2		# else make iteration with length 65535
L3:
	movc5	r0,(r1),$0,r2,(r3)	# Move n bytes, pad with nulls
	tstl	r8		# was dst.ln > 65535?
	bleq	L5		# NO, we're done
	tstl	r7		# was src.ln also > 65535?
	bgtr	L4		# YES, then go move some more
	clrl	r0		# NO, set up to pad nulls
	movl	r8,r2		#   to dst.ln `n'
	jmp	L2
L4:
	movl	r7,r0		# restore current src.len
	movl	r8,r2		# restore current dst.len
	jmp	L1		# go move some more string
L5:
	movl	r6,r0		# wer'e done - return s1
	ret
