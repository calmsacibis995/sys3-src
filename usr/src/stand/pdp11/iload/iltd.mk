TESTDIR = .
FRC =
INS = /etc/install
INSDIR = /stand
CFLAGS = -O
FILES = iltd.o tcrt0.o prf.o console.o hthp.o tmrp.o \
	rl.o rk.o \
	rp03boot.o rp04boot.o rl11boot.o rk11boot.o

all:	$(TESTDIR)/iltd

$(TESTDIR)/iltd: $(FILES)
	ld -s -o $(TESTDIR)/iltd $(FILES) -lc

rp03boot.o:	bthdr tail03
	cat bthdr /stand/rp03boot tail03 >rp03boot.o

rp04boot.o:	bthdr tail04
	cat bthdr /stand/rp04boot tail04 >rp04boot.o

rl11boot.o:	bthdr tailrl
	cat bthdr /stand/rl11boot tailrl >rl11boot.o

rk11boot.o:	bthdr tailrk
	cat bthdr /stand/rk11boot tailrk >rk11boot.o

$(FILES): $(FRC)

install: all
	$(INS) -f $(INSDIR) $(TESTDIR)/iltd

clean:
	-rm -f *.o

clobber: clean
	-rm -f $(TESTDIR)/iltd

FRC:
