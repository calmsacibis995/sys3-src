TESTDIR=.
INS= /etc/install -n /bin
INSDIR=
CFLAGS=-O
LDFLAGS = -s -n

all: cc make_cpp make_pcc make_c2
cc: cc.c make_cpp make_pcc make_c2
	$(CC) $(LDFLAGS) "$(CFLAGS)" -DCSW=3 -o $(TESTDIR)/cc cc.c
make_cpp:
	cd cpp; make -f cpp.mk LDFLAGS="$(LDFLAGS)" CFLAGS="$(CFLAGS)" TESTDIR=$(TESTDIR)
make_pcc:
	cd pcc; make -f pcc.mk LDFLAGS="$(LDFLAGS)" CFLAGS="$(CFLAGS)" TESTDIR=$(TESTDIR)
make_c2:
	cd c2; make -f c2.mk LDFLAGS="$(LDFLAGS)" CFLAGS="$(CFLAGS)" TESTDIR=$(TESTDIR)
clean :
	rm -f *.o
	cd cpp; make -f cpp.mk TESTDIR=$(TESTDIR) clean
	cd pcc; make -f pcc.mk TESTDIR=$(TESTDIR) clean
	cd c2; make -f c2.mk TESTDIR=$(TESTDIR) clean

clobber: clean
	rm -f $(TESTDIR)/cc
	cd cpp; make -f cpp.mk TESTDIR=$(TESTDIR) clobber
	cd pcc; make -f pcc.mk TESTDIR=$(TESTDIR) clobber
	cd c2; make -f c2.mk TESTDIR=$(TESTDIR) clobber

install : all
	$(INS) $(TESTDIR)/cc
	cd cpp; make -f cpp.mk LDFLAGS="$(LDFLAGS)" CFLAGS="$(CFLAGS)" TESTDIR=$(TESTDIR) install
	cd pcc; make -f pcc.mk LDFLAGS="$(LDFLAGS)" CFLAGS="$(CFLAGS)" TESTDIR=$(TESTDIR) install
	cd c2; make -f c2.mk LDFLAGS="$(LDFLAGS)" CFLAGS="$(CFLAGS)" TESTDIR=$(TESTDIR) install
FRC:
