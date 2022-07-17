TESTDIR = .
FRC =
INS = /etc/install -n /bin
INSDIR = 
CFLAGS = -O
LDFLAGS = -n -s

all: mv

mv: mv.c $(FRC)
	$(CC) $(LDFLAGS) -o $(TESTDIR)/mv $(CFLAGS) mv.c 

test:
	rtest $(TESTDIR)/mv

install: all
	cp $(TESTDIR)/mv $(TESTDIR)/ln
	PATH=$(TESTDIR):/bin:/usr/bin:/etc $(INS) $(TESTDIR)/mv $(INSDIR)
	rm -f /bin/ln
	PATH=$(TESTDIR):/bin:/usr/bin:/etc $(INS) $(TESTDIR)/ln $(INSDIR)
	./ln /bin/ln /bin/cp

clean:
	-rm -f *.o

clobber: clean
	-rm -f $(TESTDIR)/mv $(TESTDIR)/ln

FRC:
