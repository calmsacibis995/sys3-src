LIBNAME = ../lib0
INCRT = /usr/include
CFLAGS = -O $(INCRT)
FRC =

FILES =\
	$(LIBNAME)(tdump.o)

IFILES =\
	$(LIBNAME)(tdump.o)

MFILES = cons.m mba.m mtpr.m page.m pcb.m psl.m rpb.m trap.m uba.m

SFILES = rpb.s scb.s trap.s power.s random.s start.s cswitch.s userio.s \
	copy.s math.s misc.s end.s

all:	$(LIBNAME) ../locore.o

.PRECIOUS:	$(LIBNAME)

$(LIBNAME):	$(FILES) $(FRC)
	strip $(LIBNAME)

$(IFILES):
	$(CC) -I$(INCRT) -S $*.c
	/lib/c2 -i $*.s $*.os
	as -o $*.o $*.os
	ar rv $@ $*.o
	-rm -f $*.s $*.os $*.o

../locore.o:	$(SFILES) $(MFILES) $(FRC)
	cat $(MFILES) $(SFILES) | /bin/as
	-ld -r -x -o ../locore.o a.out
	-rm -f a.out

clean:
	-rm -f a.out *.o tdump.s tdump.s

clobber:	clean
	-rm -f $(LIBNAME) ../locore.o

FRC:
