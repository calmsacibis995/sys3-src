CFLAGS=-O
INS = /etc/install -n /bin
INSDIR =
TESTDIR = .
FRC =
all:	adb
adb:	access.o command.o expr.o \
	format.o input.o opset.o optab.o main.o \
	message.o output.o \
	pcs.o print.o runpcs.o setup.o sym.o
	$(CC) $(LDFLAGS) $(LDFLAGS) -s -o $(TESTDIR)/adb *.o 

defs.h:		mac.h mode.h
		touch defs.h
mode.h:		machine.h
		touch mode.h

access.o:	defs.h access.c
command.o:	defs.h command.c
expr.o:		defs.h expr.c
format.o:	defs.h format.c
input.o:	defs.h input.c
main.o:		defs.h main.c
message.o:	mac.h mode.h message.c
	$(CC) $(CFLAGS) -S message.c
	ed - <:rofix message.s
	as -o message.o message.s
	rm message.s
opset.o:	defs.h opset.c
optab.o:	defs.h /usr/src/cmd/as/vax/instrs optab.c
	$(CC) $(LDFLAGS) $(CFLAGS) -S optab.c
	ed - <:rofix optab.s
	$(AS) -o optab.o optab.s
	rm optab.s
output.o:	defs.h output.c
pcs.o:		defs.h pcs.c
print.o:	defs.h print.c
runpcs.o:	defs.h runpcs.c
setup.o:	defs.h setup.c
sym.o:		defs.h sym.c

install : all
	$(INS) $(TESTDIR)/adb

clean :
	rm -f *.o
clobber: clean
	rm -f $(TESTDIR)/adb
FRC:
