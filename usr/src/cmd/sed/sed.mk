TESTDIR = .
FRC =
INS = /etc/install -n /bin
INSDIR =
CFLAGS = -O
OFILES = sed0.o sed1.o

all: sed

sed:	$(OFILES)
	$(CC) $(LDFLAGS) $(CFLAGS) -s -n -o $(TESTDIR)/sed $(OFILES)

$(OFILES): sed.h $(FRC)

test:
	rtest $(TESTDIR)/sed

install:	all
		$(INS) $(TESTDIR)/sed $(INSDIR)

clean:
	-rm *.o

clobber:	clean
		-rm $(TESTDIR)/sed

FRC:
