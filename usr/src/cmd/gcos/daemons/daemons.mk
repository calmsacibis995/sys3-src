CFLAGS = -O
LDFLAGS = -s -n
LIBES =
INS = :
FRC =
TESTDIR = .
DEMDIR = /usr/lib

all:	daemons
daemons:	dpd fget.demon

dpd::	phone.dpd.o gerts.o gcset.o $(FRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TESTDIR)/dpd phone.dpd.o gerts.o\
		gcset.o $(LIBES)
dpd::
	$(INS) -n $(DEMDIR) $(TESTDIR)/dpd
phone.dpd.o:	phone.dpd.c daemon.c daemon0.c gcos.h $(FRC)

fget.demon::	phone.fgd.o gerts.o cgset.o $(FRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TESTDIR)/fget.demon phone.fgd.o gerts.o\
		cgset.o $(LIBES)
fget.demon::
	$(INS) -n $(DEMDIR) $(TESTDIR)/fget.demon
phone.fgd.o:	phone.fgd.c fgdemon.c daemon0.c gcos.h $(FRC)

gerts.o:	gerts.c gcos.h $(FRC)
cgset.o:	cgset.c
gcset.o:	gcset.c

install:
	make -f daemons.mk all INS=/etc/install FRC=$(FRC)\
		COMDIR=$(COMDIR) DEMDIR=$(DEMDIR) TESTDIR=$(TESTDIR)\
		"CFLAGS=$(CFLAGS)" "LDFLAGS=$(LDFLAGS)" "LIBES=$(LIBES)" 

clean:
	rm -f *.o

clobber:	clean
	cd $(TESTDIR);\
		rm -f dpd fget.demon

FRC:
