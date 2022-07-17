TESTDIR = .
FRC =
INS = /etc/install
INSDIR = 

all: prof

# If you don't want to plot, take out the -Dplot and the ref. to plot.a
prof: prof.c $(FRC)
	$(CC) $(LDFLAGS) -O -s -o $(TESTDIR)/prof prof.c  

test:
	rtest $(TESTDIR)/prof

install: all
	$(INS) $(TESTDIR)/prof $(INSDIR)

clean:
	-rm -f *.o

clobber: clean
	-rm -f $(TESTDIR)/prof

FRC:
