TESTDIR = .
FRC =
INS = /etc/install -n /usr/bin
IFLAG = -i
INSDIR =
CFLAGS = -O
FFLAG =

all: graph

graph: graph.o 
	$(CC) $(LDFLAGS) $(FFLAG) -s $(IFLAG) -o $(TESTDIR)/graph graph.o -lplot -lm 

graph.o:  graph.c $(FRC)

test:
	rtest $(TESTDIR)/graph

install: all
	$(INS) $(TESTDIR)/graph $(INSDIR)

clean:
	-rm -f *.o

clobber: clean
	-rm -f $(TESTDIR)/graph

FRC:
