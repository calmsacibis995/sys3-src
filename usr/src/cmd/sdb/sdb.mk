TESTDIR = .
FRC =
INS = /etc/install -n /usr/bin
INSDIR =
CFLAGS = -O -DSTD

all:	stest sdb

stest:	
	@set +e;if pdp11;then echo "Sdb does not run on the pdp11";exit 1;fi

decode.o:	head.h decode.c
docomm.o:	head.h docomm.c
main.o:		head.h main.c
sub.o:		head.h sub.c
re.o:		head.h re.c
fio.o:		head.h fio.c
bio.o:		bio.h bio.c
access.o:	defs.h mac.h machine.h mode.h access.c
pcs.o:		head.h defs.h mac.h machine.h mode.h pcs.c
runpcs.o:	head.h defs.h mac.h machine.h mode.h runpcs.c
xeq.o:		head.h xeq.c
setup.o:	head.h defs.h mac.h machine.h mode.h setup.c
message.o:	defs.h mac.h machine.h mode.h message.c
udef.o:		udef.c
symt.o:		bio.h head.h defs.h symt.c
display.o:	bio.h head.h defs.h cdefs.h display.c
prvar.o:	bio.h head.h defs.h cdefs.h prvar.c

sdb:	main.o re.o fio.o bio.o decode.o docomm.o sub.o head.h \
	access.o pcs.o runpcs.o setup.o message.o udef.o \
	symt.o display.o xeq.o mktemp.o version.o prvar.o
	$(CC) $(LDFLAGS) -o $(TESTDIR)/sdb *.o
	echo DONE

test:
	rtest $(TESTDIR)/sdb

install : all
	cp sdb sdb.temp
	$(INS) $(TESTDIR)/sdb $(INSDIR)
	mv sdb.temp sdb

clean :
	rm -f *.o 
clobber: clean
	rm -f $(TESTDIR)/sdb

print:
	pr *.h *.c
FRC:
