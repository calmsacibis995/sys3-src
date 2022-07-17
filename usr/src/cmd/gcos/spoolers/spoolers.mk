CFLAGS = -O
LDFLAGS = -s -n
LIBES =
INS = :
FRC =
TESTDIR = .
COMDIR = /usr/bin

all:	dpr fget fsend gcat gcosmail
spoolers:	dpr fget fsend gcat gcosmail ibm labmake tekstare

dpr::	dpr.c spool.c $(FRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TESTDIR)/dpr dpr.c $(LIBES)
dpr::
	$(INS) -n $(COMDIR) $(TESTDIR)/dpr

fget::	fget.c spool.c gcos.h $(FRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TESTDIR)/fget fget.c $(LIBES)
fget::
	$(INS) -n $(COMDIR) $(TESTDIR)/fget

fsend::	fsend.c spool.c $(FRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TESTDIR)/fsend fsend.c $(LIBES)
fsend::
	$(INS) -n $(COMDIR) $(TESTDIR)/fsend

gcat::	gcat.c spool.c $(FRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TESTDIR)/gcat gcat.c $(LIBES)
gcat::
	$(INS) -n $(COMDIR) $(TESTDIR)/gcat

gcosmail::	gcosmail.c spool.c $(FRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TESTDIR)/gcosmail gcosmail.c $(LIBES)
gcosmail::
	$(INS) -n $(COMDIR) $(TESTDIR)/gcosmail

ibm::	ibm.c spool.c $(FRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TESTDIR)/ibm ibm.c $(LIBES)
ibm::
	$(INS) -n $(COMDIR) $(TESTDIR)/ibm

labmake::	labmake.c spool.c $(FRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TESTDIR)/labmake labmake.c $(LIBES)
labmake::
	$(INS) -n $(COMDIR) $(TESTDIR)/labmake

tekstare::	tekstare.c spool.c $(FRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TESTDIR)/tekstare tekstare.c $(LIBES)
tekstare::
	$(INS) -n $(COMDIR) $(TESTDIR)/tekstare

install:
	make -f spoolers.mk all INS=/etc/install FRC=$(FRC)\
		COMDIR=$(COMDIR) DEMDIR=$(DEMDIR) TESTDIR=$(TESTDIR)\
		"CFLAGS=$(CFLAGS)" "LDFLAGS=$(LDFLAGS)" "LIBES=$(LIBES)" 

clean:
	rm -f *.o

clobber:	clean
	cd $(TESTDIR);\
	rm -f dpr fget fsend gcat gcosmail ibm labmake tekstare

FRC:
