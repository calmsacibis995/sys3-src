CFLAGS = -O
LDFLAGS = -s -n
LIBES =
INS = :
FRC =
TESTDIR = .
COMDIR = /usr/bin
DEMDIR = /usr/lib

all:
	echo cat README

spoolers:	FRC
	cd spoolers;\
	make -f spoolers.mk INS=$(INS) FRC=$(FRC)\
		COMDIR=$(COMDIR) DEMDIR=$(DEMDIR) TESTDIR=$(TESTDIR)\
		"CFLAGS=$(CFLAGS)" "LDFLAGS=$(LDFLAGS)" "LIBES=$(LIBES)" 

daemons:	FRC
	cd daemons;\
	make -f daemons.mk INS=$(INS) FRC=$(FRC)\
		COMDIR=$(COMDIR) DEMDIR=$(DEMDIR) TESTDIR=$(TESTDIR)\
		"CFLAGS=$(CFLAGS)" "LDFLAGS=$(LDFLAGS)" "LIBES=$(LIBES)" 

install:
	make -f gcos.mk all INS=/etc/install FRC=$(FRC)\
		COMDIR=$(COMDIR) DEMDIR=$(DEMDIR) TESTDIR=$(TESTDIR)\
		"CFLAGS=$(CFLAGS)" "LDFLAGS=$(LDFLAGS)" "LIBES=$(LIBES)" 

clean:
	cd spoolers; make -f spoolers.mk clean TESTDIR=$(TESTDIR)
	cd daemons; make -f daemons.mk clean TESTDIR=$(TESTDIR)

clobber:
	cd spoolers; make -f spoolers.mk clobber TESTDIR=$(TESTDIR)
	cd daemons; make -f daemons.mk clobber TESTDIR=$(TESTDIR)

FRC:

.PRECIOUS:	spoolers daemons
