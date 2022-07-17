LIBNAME = ../lib3
INCRT = /usr/include
CFLAGS = -O -I$(INCRT)
FRC =

FILES =\
	$(LIBNAME)(pcl.o)\
	$(LIBNAME)(st.o)\
	$(LIBNAME)(dqs.o)\
	$(LIBNAME)(prof.o)

all:	$(LIBNAME) 

.PRECIOUS:	$(LIBNAME)

$(LIBNAME):	$(FILES)
	strip $(LIBNAME)

clean:
	-rm -f *.o

clobber:	clean
	-rm -f $(LIBNAME)

$(LIBNAME)(pcl.o):\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/pcl.h\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/map.h\
	$(FRC)

$(LIBNAME)(st.o):\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/st.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/map.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/buf.h\
	$(FRC)

$(LIBNAME)(prof.o):\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/seg.h\
	$(INCRT)/sys/map.h\
	$(FRC)
	$(CC) -c $(CFLAGS) prof.c prfintr.s
	-ld -x -r prof.o prfintr.o
	mv a.out prof.o
	ar rv $(LIBNAME) prof.o
	-rm -f prof.o prfintr.o

$(LIBNAME)(dqs.o):\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/systm.h\
	$(INCRT)/sys/dir.h\
	$(INCRT)/sys/user.h\
	$(INCRT)/sys/dqs.h\
	$(FRC)
FRC:
