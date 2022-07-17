TESTDIR = .
INS = /etc/install -n /usr/bin
INSDIR =
FRC =
CFLAGS = -O
LDFLAGS= -s -n
all: ld
ld:: ld.vax.c
	-if vax; then $(CC) $(CFLAGS) $(LDFLAGS) -o $(TESTDIR)/ld ld.vax.c; fi
ld:: ld.pdp.c
	-if pdp11; then $(CC) $(CFLAGS) $(LDFLAGS) -o $(TESTDIR)/ld ld.pdp.c; fi
install: all
	$(INS) $(TESTDIR)/ld
clean:
	rm -f *.o
clobber: clean
	rm -f $(TESTDIR)/ld

FRC:
