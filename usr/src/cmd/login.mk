TESTDIR = .
FRC =
INS = /etc/install
INSDIR = /etc 
LDFLAGS = -O -s

all: login

login: login.c $(FRC)
	$(CC) $(LDFLAGS) -o $(TESTDIR)/login login.c  

test:
	rtest $(TESTDIR)/login

install: all
	$(INS) -o $(TESTDIR)/login $(INSDIR)

clean:
	-rm -f *.o

clobber: clean
	-rm -f $(TESTDIR)/login

FRC:
