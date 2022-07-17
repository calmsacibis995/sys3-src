SYS = unix
NODE = unix
REL = 3.0.1
VER = hpht

INCRT = /usr/include
INS = /etc/install
INSDIR = /
CFLAGS = -O -I$(INCRT)
FRC =
NAME = $(SYS)$(VER)
TYPE = vax

all:	init ../$(NAME)

init:
	cd /usr/src/uts; make -f uts.mk "INS=$(INS)" "INSDIR=$(INSDIR)" \
		"INCRT=$(INCRT)" "FRC=$(FRC)" "SYS=$(SYS)" \
		"NODE=$(NODE)" "REL=$(REL)" "VER=$(VER)" "TYPE=$(TYPE)"
	@-$(CC) $(CFLAGS) -c \
		-DSYS=\"`expr $(SYS) : '\(.\{1,8\}\)'`\" \
		-DNODE=\"`expr $(NODE) : '\(.\{1,8\}\)'`\" \
		-DREL=\"`expr $(REL) : '\(.\{1,8\}\)'`\" \
		-DVER=\"`expr $(VER) : '\(.\{1,8\}\)'`\" \
		name.c

../$(NAME): univec.o conf.o ../lib[0-9] ../locore.o linesw.o name.o
	-/bin/ld -o ../$(NAME) -e start -x -T 80000000 ../locore.o \
		 ../lib0 conf.o univec.o linesw.o ../lib[1-9] name.o
	@-chmod 744 ../$(NAME)

clean:
	cd /usr/src/uts; make -f uts.mk "INS=$(INS)" "INSDIR=$(INSDIR)" \
		"INCRT=$(INCRT)" "FRC=$(FRC)" "SYS=$(SYS)" \
		"NODE=$(NODE)" "REL=$(REL)" "VER=$(VER)" "TYPE=$(TYPE)" clean
	-rm -f *.o

clobber:	clean
	cd /usr/src/uts; make -f uts.mk "INS=$(INS)" "INSDIR=$(INSDIR)" \
		"INCRT=$(INCRT)" "FRC=$(FRC)" "SYS=$(SYS)" \
		"NODE=$(NODE)" "REL=$(REL)" "VER=$(VER)" "TYPE=$(TYPE)" clobber
	-rm -f ../$(NAME) univec.c conf.c

conf.c:\
	$(INCRT)/sys/acct.h\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/callo.h\
	$(INCRT)/sys/conf.h\
	$(INCRT)/sys/elog.h\
	$(INCRT)/sys/err.h\
	$(INCRT)/sys/file.h\
	$(INCRT)/sys/inode.h\
	$(INCRT)/sys/io.h\
	$(INCRT)/sys/map.h\
	$(INCRT)/sys/mount.h\
	$(INCRT)/sys/param.h\
	$(INCRT)/sys/peri.h\
	$(INCRT)/sys/proc.h\
	$(INCRT)/sys/space.h\
	$(INCRT)/sys/sysinfo.h\
	$(INCRT)/sys/text.h\
	$(INCRT)/sys/tty.h\
	$(INCRT)/sys/uba.h\
	$(INCRT)/sys/var.h\
	$(FRC)

name.c:\
	$(INCRT)/sys/utsname.h\
	$(FRC)

univec.c:\
	$(FRC)

linesw.c:\
	$(INCRT)/sys/conf.h\
	$(FRC)

install:	all
	$(INS) -f $(INSDIR) "../$(SYS)$(VER)"

FRC:
