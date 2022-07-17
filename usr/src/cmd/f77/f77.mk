TESTDIR = .
FRC =
INSDIR =
LDFLAGS = -s
IFLAG = -i

all:
	-if vax ; then cd vax; make -f f77.mk LDFLAGS="$(LDFLAGS)" TESTDIR=$(TESTDIR) ; \
	else cd pdp11; make -f f77.mk LDFLAGS="$(LDFLAGS)" IFLAG=$(IFLAG) TESTDIR=$(TESTDIR) ; fi
install:
	-if vax ; then cd vax; make -f f77.mk LDFLAGS="$(LDFLAGS)" TESTDIR=$(TESTDIR) install; \
	else cd pdp11; make -f f77.mk LDFLAGS="$(LDFLAGS)" IFLAG=$(IFLAG) TESTDIR=$(TESTDIR) install ; fi
clean:
	-if vax ; then cd vax; make -f f77.mk clean TESTDIR=$(TESTDIR);\
	else cd pdp11; make -f f77.mk clean TESTDIR=$(TESTDIR); fi
clobber:
	-if vax ; then cd vax; make -f f77.mk clobber TESTDIR=$(TESTDIR); \
	else cd pdp11; make -f f77.mk clobber TESTDIR=$(TESTDIR); fi
