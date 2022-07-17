TESTDIR=.
INS= /etc/install -n /bin
INSDIR=
CFLAGS=-O
LDFLAGS = -s -n

all: pcc make_pcc
pcc: cc.c
	$(CC) $(LDFLAGS) "$(CFLAGS)" -DCSW=1 -'DCCNAME="pcc"' -o $(TESTDIR)/pcc cc.c
make_pcc:
	cd mdp; make -f pcc.mk LDFLAGS="$(LDFLAGS)" CFLAGS="$(CFLAGS)" TESTDIR=$(TESTDIR) comp
clean :
	rm -f *.o
	cd mdp; make -f pcc.mk TESTDIR=$(TESTDIR) clean

clobber: clean
	rm -f $(TESTDIR)/pcc
	cd mdp; make -f pcc.mk TESTDIR=$(TESTDIR) clobber

install : all
	$(INS) $(TESTDIR)/pcc
	cd mdp; make -f pcc.mk LDFLAGS="$(LDFLAGS)" CFLAGS="$(CFLAGS)" TESTDIR=$(TESTDIR) install
FRC:
