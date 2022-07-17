TESTDIR = .
FRC =
INSDIR =
CFLAGS = -O
LDFLAGS = -n -s

all:
	-if vax ;\
	then cd vax; make -f as.mk CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" TESTDIR=$(TESTDIR) ; \
	else cd pdp11; make -f as.mk CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" TESTDIR=$(TESTDIR) ; fi
install:
	-if vax ;\
	then cd vax; make -f as.mk CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" TESTDIR=$(TESTDIR) install; \
	else cd pdp11; make -f as.mk CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" TESTDIR=$(TESTDIR) install; fi
clean:
	-if vax ; then cd vax; make -f as.mk clean TESTDIR=$(TESTDIR);\
	else cd pdp11; make -f as.mk clean TESTDIR=$(TESTDIR); fi
clobber:
	-if vax ; then cd vax; make -f as.mk clobber TESTDIR=$(TESTDIR); \
	else cd pdp11; make -f as.mk clobber TESTDIR=$(TESTDIR); fi
