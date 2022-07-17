LIBNAME = ../lib1
INCRT = /usr/include
CFLAGS = -O -I$(INCRT)
FRC =
FILES =\
	$(LIBNAME)(main.o)\
	$(LIBNAME)(trap.o)\
	$(LIBNAME)(sysent.o)\
	$(LIBNAME)(sys1.o)\
	$(LIBNAME)(sys2.o)\
	$(LIBNAME)(sys3.o)\
	$(LIBNAME)(sys4.o)\
	$(LIBNAME)(acct.o)\
	$(LIBNAME)(alloc.o)\
	$(LIBNAME)(clock.o)\
	$(LIBNAME)(errlog.o)\
	$(LIBNAME)(fio.o)\
	$(LIBNAME)(iget.o)\
	$(LIBNAME)(machdep.o)\
	$(LIBNAME)(malloc.o)\
	$(LIBNAME)(nami.o)\
	$(LIBNAME)(pipe.o)\
	$(LIBNAME)(prf.o)\
	$(LIBNAME)(rdwri.o)\
	$(LIBNAME)(sig.o)\
	$(LIBNAME)(slp.o)\
	$(LIBNAME)(subr.o)\
	$(LIBNAME)(text.o)\
	$(LIBNAME)(utssys.o)

OPFILES =\
	slp

all:	$(LIBNAME) 

.PRECIOUS:	$(LIBNAME)

$(LIBNAME):	$(FILES)
	strip $(LIBNAME)

.c.a:
	@-case $* in \
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

$(LIBNAME)(acct.o):\
	$(INCRT)/sys/acct.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(alloc.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/fblk.h\
	$(INCRT)/sys/filsys.h\
	$(INCRT)/sys/ino.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/mount.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(clock.o):\
	$(INCRT)/sys/callo.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/psl.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/text.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(errlog.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/erec.h\
	$(INCRT)/sys/err.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/map.h\
	$(INCRT)/sys/mba.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/utsname.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(fio.o):\
	$(INCRT)/sys/acct.h\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/filsys.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(iget.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/filsys.h\
	$(INCRT)/sys/ino.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/mount.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(machdep.o):\
	$(INCRT)/sys/clock.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/map.h\
	$(INCRT)/sys/mtpr.h\
	$(INCRT)/sys/page.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/reg.h\
	$(INCRT)/sys/seg.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/utsname.h\
	$(FRC)

$(LIBNAME)(main.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/clock.h\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/filsys.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/mount.h\
	$(INCRT)/sys/mtpr.h\
	$(INCRT)/sys/page.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/seg.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(malloc.o):\
	$(INCRT)/sys/map.h\
	$(INCRT)/sys/mem.h\
	$(INCRT)/sys/page.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/systm.h\
	$(FRC)

$(LIBNAME)(nami.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/mount.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(pipe.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(prf.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/systm.h\
	$(FRC)

$(LIBNAME)(rdwri.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(sig.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/mtpr.h\
	$(INCRT)/sys/page.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/psl.h\
	$(INCRT)/sys/reg.h\
	$(INCRT)/sys/seg.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/text.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(slp.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/map.h\
	$(INCRT)/sys/mtpr.h\
	$(INCRT)/sys/page.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/text.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(subr.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(sys1.o):\
	$(INCRT)/sys/acct.h\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/map.h\
	$(INCRT)/sys/mtpr.h\
	$(INCRT)/sys/page.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/reg.h\
	$(INCRT)/sys/seg.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(sys2.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(sys3.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/filsys.h\
	$(INCRT)/sys/ino.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/mount.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/stat.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(sys4.o):\
	$(INCRT)/sys/clock.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/mtpr.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(sysent.o):\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/systm.h\
	$(FRC)

$(LIBNAME)(text.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/map.h\
	$(INCRT)/sys/page.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/seg.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/text.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(trap.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/psl.h\
	$(INCRT)/sys/reg.h\
	$(INCRT)/sys/seg.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/trap.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(utssys.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/filsys.h\
	$(INCRT)/sys/mount.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/pcb.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/utsname.h\
	$(INCRT)/sys/var.h\
	$(FRC)
