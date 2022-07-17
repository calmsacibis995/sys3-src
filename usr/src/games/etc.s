/ etc.s -- common code

	sys	48.; 2; done
	jmp	around

done:
	jsr	pc,nline
	sys	exit

mesg:
	movb	(r5)+,r0
	beq	1f
	jsr	pc,putc
	br	mesg
1:
	inc	r5
	bic	$1,r5
	rts	r5

quest:
	jsr	pc,getc
	cmp	r0,$'y
	bne	1f
	tst	(r5)+
1:
	cmp	r0,$'\n
	beq	1f
	jsr	pc,flush
1:
	rts	r5

getc:
	clr	r0
	sys	read; ch; 1
	bes	done
	tst	r0
	beq	done
	mov	ch,r0
	rts	pc

print:
	mov	r5,0f
	sys	open; 0:..; 0
	bes	1f
	mov	r0,r1
2:
	mov	r1,r0
	sys	read; ch; 1
	tst	r0
	beq	2f
	mov	ch,r0
	jsr	pc,putc
	br	2b
2:
	mov	r1,r0
	sys	close
1:
	tstb	(r5)+
	bne	1b
	inc	r5
	bic	$1,r5
	rts	r5

nline:
	mov	$'\n,r0

putc:
	mov	r0,ch
	mov	$1,r0
	sys	write; ch; 1
	rts	pc

decml:
	mov	r1,-(sp)
	jsr	pc,1f
	mov	(sp)+,r1
	rts	pc

1:
	mov	r0,r1
	clr	r0
	div	$10.,r0
	mov	r1,-(sp)
	tst	r0
	beq	1f
	jsr	pc,1b
1:
	mov	(sp)+,r0
	add	$'0,r0
	jsr	pc,putc
	rts	pc

flush:
	jsr	pc,getc
	cmp	r0,$'\n
	bne	flush
	rts	pc

rand:
	mov	r1,-(sp)
	tst	randx
	bne	1f
	sys	time
	mov	r1,randx
	bis	$1,randx
1:
	mov	randx,r0
	mul	$15625.,r0
	mov	r1,randx
	ashc	$-2,r0
	clr	r0
	div	(r5)+,r0
	mov	r1,r0
	mov	(sp)+,r1
	rts	r5

ch:	0
randx:	0
around:

