TESTDIR = .
FRC =
INS = :
ASM = as0.o as1.o as2.o as3.o rodata.o
CFLAGS = -O

all:	as

as::	$(ASM)
	$(CC) -o as $(ASM)

as::
	$(INS) $(TESTDIR)/as

$(ASM):	$(FRC)

test:
	rtest $(TESTDIR)/as

install:	all
	make -f as.mk all INS=/etc/install

clean:
	-rm -f *.o as0.c as?.s as.yh rodata.[cso]

clobber:	clean
	-rm -f $(TESTDIR)/as

as0.c:	as0.y
	yacc -d as0.y
	mv y.tab.h as.yh
	rm -f rodata.c
	./:yyfix yyexca yyact yypact yypgo yyr1 yyr2 yychk yydef
	mv y.tab.c as0.c
	$(CC) $(CFLAGS) -S rodata.c
	./:rofix rodata.s
	$(AS) -o rodata.o rodata.s

FRC:
