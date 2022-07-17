TESTDIR = .
INS = /etc/install
INSDIR =
FRC =
CFLAGS=-O
LDFLAGS=-s

c2:	c20.o c21.o c22.o nargs.o
	$(CC) $(LDFLAGS) -o $(TESTDIR)/c2 c2?.o nargs.o

mon.o: /usr/lib/mon.c
	$(CC) $(CFLAGS) -c -p -DMASTER='"/usr/lib/c2.monsum"' -DINCREMENT='"/tmp/c2XXXXX"' /usr/lib/mon.c
	
c20.o:	c20.c c2.h
c21.o:	c21.c c2.h
c22.o:  c22.c c2.h
	$(CC) $(CFLAGS) -S c22.c
	/usr/src/cmd/as/vax/:rofix c22.s
	$(AS) -o c22.o c22.s

nargs.o: nargs.s
	$(AS) -o nargs.o nargs.s

clean :
	rm -f *.o c22.s
clobber: clean
	rm -f $(TESTDIR)/c2 $(TESTDIR)/nc2

install : c2
	rm -f /lib/nc2
	cp /lib/c2 /lib/nc2
	ln c2 nc2
	$(INS) $(TESTDIR)/nc2 $(DESTDIR)/lib
	rm nc2
