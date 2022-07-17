CFL = -DHERE=PDP11 -DTARGET=PDP11 -DFAMILY=DMR -DNEWDMR

TESTDIR = .
INS = /etc/install
INSDIR =
FRC =
CFLAGS = -O $(CFL)
LDFLAGS = -s
IFLAG = -i

OBJECTS = main.o fltused.o init.o gram.o lex.o proc.o equiv.o data.o \
	  expr.o exec.o intr.o io.o misc.o error.o put.o \
	  putdmr.o pdp11.o pdp11x.o


all:	itest compiler

itest:
	@if [ x$(IFLAG) != x-i ] ; then echo NO ID SPACE ; exit 1 ; else exit 0 ; fi

compiler:: f77 f77pass1
	touch compiler

compiler:: /lib/c1
	@echo "***Warning: /lib/c1 has changed"

f77 : driver.o pdp11x.o
	$(CC) -n driver.o pdp11x.o -o $(TESTDIR)/f77

f77pass1 : $(OBJECTS)
	@echo LOAD
	$(CC) $(LDFLAGS) $(IFLAG) $(OBJECTS) -o $(TESTDIR)/f77pass1
	@size f77pass1

gram.c:	gram.head gram.dcl gram.expr gram.exec gram.io tokdefs
	( sed <tokdefs "s/#define/%token/" ;\
		cat gram.head gram.dcl gram.expr gram.exec gram.io ) >gram.in
	$(YACC) $(YFLAGS) gram.in
	@echo "(expect 4 shift/reduce)"
	mv y.tab.c gram.c
	rm gram.in

tokdefs: tokens
	grep -n . <tokens | sed "s/\([^:]*\):\(.*\)/#define \2 \1/" >tokdefs
lex.o : tokdefs
driver.o $(OBJECTS)  : defs defines machdefs ftypes
driver.o : drivedefs
io.o : fio.h

machdefs : pdp11defs
	cp pdp11defs machdefs

put.o putdmr.o pdp11.o : dmrdefs

install: all
	$(INS) -n /usr/bin $(TESTDIR)/f77 $(INSDIR)
	$(INS) -n /usr/lib $(TESTDIR)/f77pass1 $(INSDIR)

clean:
	-rm -f gram.c *.o

clobber: clean
	-rm -f $(TESTDIR)/f77 $(TESTDIR)/f77pass1

FRC:

