TESTDIR = .
INS = /etc/install
FRC =
CFLAGS = -O
IFLAG = -i
FILES = backspace.x dfe.x due.x iio.x inquire.x \
	lib.x rewind.x rsfe.x \
	rdfmt.x sue.x uio.x wsfe.x sfe.x fmt.x \
	lio.x lread.x open.x close.x util.x \
	endfile.x wrtfmt.x err.x fmtlib.x

all:	libI77.a


libI77.a:	$(FILES) $(FRC)

lio.h:
	chmod 0666 lio.h
lio.x lread.x:	lio.h
fmt.x rdfmt.x rsfe.x testfmt.x wrtfmt.x wsfe.x:	fmt.h
$(FILES):	fio.h  /usr/include/stdio.h/

install: all
	 $(INS) $(TESTDIR)/libI77.a

clean:
	 -rm -f *.o *.x

clobber: clean
	 -rm -f $(TESTDIR)/libI77.a

FRC:

.SUFFIXES:	.o .x .c
.c.o:
	$(CC) $(CFLAGS) -c $<
.c.x:
	$(CC) $(CFLAGS) -c $<
	ar ru $(TESTDIR)/libI77.a $*.o
	> $*.x
