TESTDIR = .
FRC =
INS = :
LDFLAGS = -n -s
AS1 = as11.s as12.s as13.s as14.s as15.s as16.s as17.s as18.s as19.s
AS2 = as21.s as22.s as23.s as24.s as25.s as26.s as27.s as28.s as29.s

all:	as2 as

as::	$(AS1)
	as -o as1.o /usr/include/sys.s $(AS1)
	ld $(LDFLAGS) -o $(TESTDIR)/as as1.o
as::
	$(INS) $(TESTDIR)/as

as2::	$(AS2)
	as -o as2.o /usr/include/sys.s $(AS2)
	ld $(LDFLAGS) -o $(TESTDIR)/as2 as2.o
as2::
	$(INS) $(TESTDIR)/as2 /lib

$(AS1) $(AS2):	$(FRC)

test:
	rtest $(TESTDIR)/as

install:	all
	make -f as.mk all INS=/etc/install

clean:
	-rm -f *.o

clobber:	clean
	-rm -f $(TESTDIR)/as $(TESTDIR)/as2

FRC:
