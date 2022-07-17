TESTDIR = .
FRC =
INS = /etc/install -n /usr/bin
INSDIR =
CFLAGS = -O
IFLAG = -i

all: dc

dc:
	if [ "$(IFLAG)" ] ; then $(CC) -O -s -n -o $(TESTDIR)/dc dc.c ;\
	else $(CC) -s -n -o $(TESTDIR)/dc dc.c ; fi

dc.o: dc.h $(FRC)

test:
	rtest $(TESTDIR)/dc

install: all
	$(INS) $(TESTDIR)/dc $(INSDIR)

clean:
	-rm -f *.o

clobber: clean
	-rm -f $(TESTDIR)/dc

FRC:
