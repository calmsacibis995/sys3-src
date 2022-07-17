LIBNAME = ../lib2
INCRT = /usr/include
CFLAGS = -O -I$(INCRT)
FRC =

FILES =\
	$(LIBNAME)(bio.o)\
	$(LIBNAME)(tty.o)\
	$(LIBNAME)(tt0.o)\
	$(LIBNAME)(clist.o)\
	$(LIBNAME)(cons.o)\
	$(LIBNAME)(dl.o)\
	$(LIBNAME)(dn.o)\
	$(LIBNAME)(dz.o)\
	$(LIBNAME)(dza.o)\
	$(LIBNAME)(dzb.o)\
	$(LIBNAME)(dmb.o)\
	$(LIBNAME)(dmc.o)\
	$(LIBNAME)(dmr.o)\
	$(LIBNAME)(err.o)\
	$(LIBNAME)(hp.o)\
	$(LIBNAME)(ht.o)\
	$(LIBNAME)(mba.o)\
	$(LIBNAME)(mem.o)\
	$(LIBNAME)(sys.o)\
	$(LIBNAME)(uba.o)\
	$(LIBNAME)(partab.o)\
	$(LIBNAME)(lp.o)\
	$(LIBNAME)(pio.o)\
	$(LIBNAME)(kmc.o)\
	$(LIBNAME)(vp.o)\
	$(LIBNAME)(pwr.o)\
	$(LIBNAME)(vpmt.o)\
	$(LIBNAME)(vpmb.o)\
	$(LIBNAME)(trace.o)

IFILES =\
	dl|\
	dmb|\
	dmc|\
	dmr|\
	dn|\
	dz|\
	dza|\
	dzb|\
	hp|\
	ht|\
	kmc|\
	lp|\
	vp|\
	uba

OPFILES =\
	bio|\
	clist

all:	$(LIBNAME) 

.PRECIOUS:	$(LIBNAME)

$(LIBNAME):	$(FILES)
	strip $(LIBNAME)

.c.a:
	@-case $* in \
	$(IFILES))\
		echo "\t$(CC) -I$(INCRT) -S $<";\
		$(CC) -I$(INCRT) -S $<;\
		echo "\t/lib/c2 -i $*.s $*.os";\
		/lib/c2 -i $*.s $*.os;\
		echo "\tas -o $*.o $*.os";\
		as -o $*.o $*.os;\
		echo "\trm -f $*.s $*.os";\
		rm -f $*.s $*.os;\
		;;\
	$(OPFILES))\
		echo "\t$(CC) -I$(INCRT) -S $<";\
		$(CC) -I$(INCRT) -S $<;\
		echo "\ted - $*.s <../spl.ed";\
		ed - $*.s <../spl.ed;\
		echo "\t/lib/c2 $*.s $*.os";\
		/lib/c2 $*.s $*.os;\
		echo "\tas -o $*.o $*.os";\
		as -o $*.o $*.os;\
		echo "\trm -f $*.s $*.os";\
		rm -f $*.s $*.os;\
		;;\
	*)\
		echo "\t$(CC) -c $(CFLAGS) $<";\
		$(CC) -c $(CFLAGS) $<;\
		;;\
	esac
	ar rv $@ $*.o
	-rm -f $*.o

clean:
	-rm -f *.o

clobber:	clean
	-rm -f $(LIBNAME)

$(LIBNAME)(bio.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/page.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/seg.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(clist.o):\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/tty.h\
	$(FRC)

$(LIBNAME)(cons.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/cons.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/mtpr.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dl.o):\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dmb.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dmc.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dmr.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dn.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dz.o):\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dza.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/uba.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dzb.o):\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/uba.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(err.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/erec.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/mba.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/utsname.h\
	$(FRC)

$(LIBNAME)(hp.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/mba.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(ht.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/mba.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(kmc.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(lp.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(mba.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/mba.h\
	$(INCRT)/sys/page.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/seg.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(mem.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/mtpr.h\
	$(INCRT)/sys/page.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(partab.o):\
	$(FRC)

$(LIBNAME)(pio.o):\
	$(INCRT)/sys/mtpr.h\
	$(INCRT)/sys/page.h\
	$(INCRT)/sys/param.h\
	$(FRC)

$(LIBNAME)(pwr.o):\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/systm.h\
	$(FRC)

$(LIBNAME)(sys.o):\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(trace.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(tt0.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(tty.o):\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/ttold.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(uba.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/map.h\
	$(INCRT)/sys/page.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/uba.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(vp.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/peri.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(vpmb.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/vpm.h\
	$(INCRT)/sys/vpmd.h\
	$(FRC)

$(LIBNAME)(vpmt.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/vpm.h\
	$(INCRT)/sys/vpmd.h\
	$(FRC)

FRC:
