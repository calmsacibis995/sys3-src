SYS = unix
NODE = unix
REL = 3.0.1
VER = hpht

TYPE = id

INS = /etc/install
INSDIR = /
INCRT = /usr/include
CFLAGS = -O -I$(INCRT)
FRC =
NAME = $(SYS)$(VER)

all:	init ../$(NAME)

init:
	@if [ $(TYPE) = i ];\
		then\
		grep -v .data < low.s > xlow.s;\
		mv xlow.s low.s;\
	fi
	cd /usr/src/uts; make -f uts.mk "INS=$(INS)" "INSDIR=$(INSDIR)" \
		"INCRT=$(INCRT)" "FRC=$(FRC)" "SYS=$(SYS)" \
		"NODE=$(NODE)" "REL=$(REL)" "VER=$(VER)" "TYPE=$(TYPE)"
	@-$(CC) $(CFLAGS) -c -DSYS=\"`expr $(SYS) : '\(.\{1,8\}\)'`\" \
		-DNODE=\"`expr $(NODE) : '\(.\{1,8\}\)'`\" \
		-DREL=\"`expr $(REL) : '\(.\{1,8\}\)'`\" \
		-DVER=\"`expr $(VER) : '\(.\{1,8\}\)'`\" name.c

../$(NAME): low.o conf.o ../mch_$(TYPE).o ../lib[0-9] linesw.o name.o
	@-case $(TYPE) in\
		i)\
			echo "\t/bin/ld -x low.o ../mch_$(TYPE).o ../lib0 conf.o"\
				linesw.o ../lib[1-9] name.o -o ../$(NAME);\
			/bin/ld -x low.o ../mch_$(TYPE).o ../lib0 conf.o\
				linesw.o ../lib[1-9] name.o -o ../$(NAME);\
			;;\
		id)\
			echo "\t/bin/ld -rdx low.o ../mch_$(TYPE).o ../lib0 conf.o"\
				linesw.o ../lib[1-9] name.o;\
			/bin/ld -rdx low.o ../mch_$(TYPE).o ../lib0 conf.o\
				linesw.o ../lib[1-9] name.o;\
			echo "\t/etc/sysfix a.out ../$(NAME)";\
			/etc/sysfix a.out ../$(NAME);\
			rm -f a.out;\
			;;\
	esac

clean:
	cd /usr/src/uts; make -f uts.mk "INS=$(INS)" "INSDIR=$(INSDIR)" \
		"INCRT=$(INCRT)" "FRC=$(FRC)" "SYS=$(SYS)" \
		"NODE=$(NODE)" "REL=$(REL)" "VER=$(VER)" "TYPE=$(TYPE)" clean
	-rm -f *.o

clobber:	clean
	cd /usr/src/uts; make -f uts.mk "INS=$(INS)" "INSDIR=$(INSDIR)" \
		"INCRT=$(INCRT)" "FRC=$(FRC)" "SYS=$(SYS)" \
		"NODE=$(NODE)" "REL=$(REL)" "VER=$(VER)" "TYPE=$(TYPE)" clobber
	-rm -f *.o

conf.c:\
	$(INCRT)/sys/acct.h\
	$(INCRT)/sys/buf.h\
	$(INCRT)/sys/callo.h\
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
	$(INCRT)/sys/var.h\
	$(FRC)

linesw.c:\
	$(INCRT)/sys/conf.h\
	$(FRC)

low.s:\
	$(FRC)

name.c:\
	$(INCRT)/sys/utsname.h\
	$(FRC)

install:	all
	$(INS) -f $(INSDIR) "../$(SYS)$(VER)"

FRC:
