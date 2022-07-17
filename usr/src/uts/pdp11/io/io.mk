LIBNAME = ../lib2
INCRT = /usr/include
CFLAGS = -O -I$(INCRT)
FRC =

FILES =\
	$(LIBNAME)(bio.o)\
	$(LIBNAME)(cat.o)\
	$(LIBNAME)(da.o)\
	$(LIBNAME)(dh.o)\
	$(LIBNAME)(dl.o)\
	$(LIBNAME)(dmb.o)\
	$(LIBNAME)(dmc.o)\
	$(LIBNAME)(dmr.o)\
	$(LIBNAME)(dn.o)\
	$(LIBNAME)(du.o)\
	$(LIBNAME)(dz.o)\
	$(LIBNAME)(dza.o)\
	$(LIBNAME)(dzb.o)\
	$(LIBNAME)(err.o)\
	$(LIBNAME)(hp.o)\
	$(LIBNAME)(hs.o)\
	$(LIBNAME)(ht.o)\
	$(LIBNAME)(kl.o)\
	$(LIBNAME)(kmc.o)\
	$(LIBNAME)(lp.o)\
	$(LIBNAME)(mem.o)\
	$(LIBNAME)(partab.o)\
	$(LIBNAME)(pwr.o)\
	$(LIBNAME)(rf.o)\
	$(LIBNAME)(rk.o)\
	$(LIBNAME)(rl.o)\
	$(LIBNAME)(rp.o)\
	$(LIBNAME)(sys.o)\
	$(LIBNAME)(tm.o)\
	$(LIBNAME)(tt0.o)\
	$(LIBNAME)(tty.o)\
	$(LIBNAME)(vp.o)\
	$(LIBNAME)(vpmt.o)\
	$(LIBNAME)(vpmb.o)\
	$(LIBNAME)(trace.o)

all:	$(LIBNAME) 

.PRECIOUS:	$(LIBNAME)

$(LIBNAME):	$(FILES)
	strip $(LIBNAME)

clean:
	-rm -f *.o

clobber:	clean
	-rm -f $(LIBNAME)

$(LIBNAME)(bio.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/seg.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/var.h\
	$(FRC)

$(LIBNAME)(cat.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(da.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/peri.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dh.o):\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dl.o):\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dmb.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dmc.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dmr.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dn.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(du.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/peri.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dz.o):\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(dzb.o):\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/tty.h\
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
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/utsname.h\
	$(FRC)

$(LIBNAME)(hp.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(hs.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(ht.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(kl.o):\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(kmc.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(lp.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(mem.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(n.dh.o):\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(partab.o):\
	$(FRC)

$(LIBNAME)(pwr.o):\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/systm.h\
	$(FRC)

$(LIBNAME)(rf.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(rk.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(rl.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(rp.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(sys.o):\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(tm.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/iobuf.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(trace.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(tt0.o):\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/param.h\
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
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/ttold.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(vp.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/ioctl.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/peri.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/user.h\
	$(FRC)

$(LIBNAME)(vpmb.o):\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/param.h\
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
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/vpm.h\
	$(INCRT)/sys/vpmd.h\
	$(FRC)

FRC:
