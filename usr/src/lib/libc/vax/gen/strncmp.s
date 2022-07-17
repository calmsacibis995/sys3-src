# Fast assembler language version of the following C-program
#			strncmp
# which represents the `standard' for the C-library.

# Compare strings (at most n bytes): return *s1-*s2 for the last
# characters in s1 and s2 which were compared.

#	strncmp(s1, s2, n)
#	register char *s1, *s2;
#	register n;
#	{
#		while (--n >= 0 && *s1 == *s2++)
#			if (*s1++ == '\0')
#				return(0);
#		return(n<0 ? 0 : *s1 - *--s2);
#	}
	.globl	_strncmp
	.text
	.align	1
_strncmp:
	.word	0x00c0		# uses regs. 0,1,2,3,4,5,6,7
	movl	12(ap),r5	# arg `n'
	bgtr	L0		#      should be > 0
	clrl	r0		# else return a zero
	ret
L0:
	movq	4(ap),r6	# s1 in r6, s2 in r7
	movl	r6,r1
L1:
	locc	$0,$65535,(r1)	# search for terminal null in s1
	beql	L1		# no null yet, go back for more
	subl3	r6,r1,r4	# length of s1
	incl	r4		# add 1 to account for the null
	cmpl	r5,r4		# eval (`n' - len(s1))
	bgeq	L2		# skip if len(s1) in r4 is <= `n'
	movl	r5,r4		# else `n' is smaller, use it
L2:
	movl	r6,r1		# working copy of s1 for the cmpc3
	movl	r7,r3		# working copy of s2 for the cmpc3
L3:
	movl	r4,r0		# working copy of length for cmpc3
	subl3	$65535,r0,r4	# gotta know if working length > 65535
	bleq	L4		# skip if length <= 65535
	movl	$65535,r0	# else use 65535 for length
L4:
	cmpc3	r0,(r1),(r3)	# Compare s1 and s2
	beql	L5		# skip if strings are ==, (so far)
	subb3	(r3),(r1),r0	# else, difference returned
	cvtbl	r0,r0		#	as an int
	ret
L5:
	tstl	r4		# this will be <0 if we're finished
	bgtr	L3		# if more compare is needed, go back
	ret			# note that r0 is already 0 in this case
