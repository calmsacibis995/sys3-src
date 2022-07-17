TESTDIR=.
INS= /etc/install
INSDIR=
CFLAGS=-O -Dunix=1 -Dvax=1

cpp :	cpp.o cpy.o rodata.o
	$(CC) $(LDFLAGS) -o $(TESTDIR)/cpp cpp.o rodata.o cpy.o  

cpy.c : cpy.y
	$(YACC) cpy.y
	../pcc/:yyfix yyexca yyact yypact yypgo yyr1 yyr2 yychk yydef
	mv y.tab.c cpy.c
cpy.o :	cpy.c yylex.c
rodata.o : cpy.c
	$(CC) $(CFLAGS) -S rodata.c
	../pcc/:rofix rodata.s
	as -o rodata.o rodata.s

clean :
	rm -f *.s *.o
clobber: clean
	rm -f cpy.c rodata.c $(TESTDIR)/cpp

install :
	$(INS) cpp $(DESTDIR)/lib
