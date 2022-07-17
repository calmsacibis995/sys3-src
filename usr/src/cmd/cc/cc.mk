TESTDIR = .
FRC =
INSDIR =
CFLAGS = -O
IFLAG = -i
LDFLAGS = -n -s
FFLAG =
YACCRM=-rm

all:
	-if vax ; then cd vax; make -f cc.mk CFLAGS="$(CFLAGS)" \
	LDFLAGS="$(LDFLAGS)" TESTDIR=$(TESTDIR) FFLAG=$(FFLAG) all ; \
	else cd pdp11; make -f cc.mk CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" \
	IFLAG=$(IFLAG) FFLAG=$(FFLAG) TESTDIR=$(TESTDIR) all ; fi
	-if pdp11 ; then cd pcc; make -f cc.mk CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" \
	IFLAG=$(IFLAG) FFLAG=$(FFLAG) TESTDIR=$(TESTDIR) all ; fi
install:
	-if vax ; then cd vax; make -f cc.mk CFLAGS="$(CFLAGS)" \
	LDFLAGS="$(LDFLAGS)" TESTDIR=$(TESTDIR) FFLAG=$(FFLAG) install ; \
	else cd pdp11; make -f cc.mk CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" \
	IFLAG=$(IFLAG) FFLAG=$(FFLAG) TESTDIR=$(TESTDIR) install ; fi
	-if pdp11 ; then cd pcc; make -f cc.mk CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" \
	IFLAG=$(IFLAG) FFLAG=$(FFLAG) TESTDIR=$(TESTDIR) install ; fi
clean:
	-if vax  ; then cd vax; make -f cc.mk clean TESTDIR=$(TESTDIR) \
	YACCRM=$(YACCRM) ; \
	else cd pdp11; make -f cc.mk clean TESTDIR=$(TESTDIR) \
	YACCRM=$(YACCRM) ; fi
	-if pdp11 ; then cd pcc; make -f cc.mk clean TESTDIR=$(TESTDIR) \
	YACCRM=$(YACCRM) ; fi
clobber:
	-if vax  ; then cd vax; make -f cc.mk clobber TESTDIR=$(TESTDIR) \
	YACCRM=$(YACCRM) ; \
	else cd pdp11; make -f cc.mk clobber TESTDIR=$(TESTDIR) \
	YACCRM=$(YACCRM) ; fi
	-if pdp11 ; then cd pcc; make -f cc.mk clobber TESTDIR=$(TESTDIR) \
	YACCRM=$(YACCRM) ; fi
