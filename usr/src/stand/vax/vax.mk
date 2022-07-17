INS = :
CFLAGS = -O
EXECS = ls du cat dd fsck od

all:	$(EXECS) iltd sash vrmhead boots conflp rp6fmt fsdb
iltd::	iltd.o	rpboot.o
	scc -s -o iltd iltd.o rpboot.o
iltd::
	$(INS) -f /stand iltd

sash::	sash0.o sash1.o
	ld -s -T 50000 sash0.o sash1.o -l2 -o sash
sash::
	$(INS) -f /stand sash

vrmhead::	vrmhead.c
	cc -s $(CFLAGS) -o vrmhead vrmhead.c
vrmhead::
	$(INS) -f /stand vrmhead

boots::
	cd boots; make -f boots.mk all INS=$(INS)

conflp::
	cd conflp; make -f conflp.mk all INS=$(INS)

$(EXECS)::
	scc -s -O -o $@ /usr/src/cmd/$@.c
$(EXECS)::
	$(INS) -f /stand $@

rp6fmt::
	scc -s -o  rp6fmt /usr/src/cmd/rp6fmt.c
rp6fmt::
	$(INS) -f /stand rp6fmt

fsdb::
	scc -s -o  fsdb /usr/src/cmd/fsdb.c
fsdb::
	$(INS) -f /stand fsdb

rpboot.o:
	cd boots; make -f boots.mk rpboot.o
	cp boots/rpboot.o .

install:
	make -f vax.mk all INS=/etc/install

clean:
	rm -f iltd.o rpboot.o
	rm -f sash?.o

clobber: clean
	rm -f sash
	rm -f vrmhead
	rm -f $(EXECS) rp6fmt fsdb
	cd boots; make -f boots.mk clobber
	rm -f iltd
