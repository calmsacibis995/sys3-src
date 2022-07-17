INS = :
CFLAGS = -O
EXECS = ls du cat dd fsck od 

all:	$(EXECS) boot sashp iltd rp6fmt fsdb mmtst

sashp:
	cd sash; make -f sash.mk INS=$(INS)

boot:
	cd boots; make -f boots.mk all INS=$(INS)

$(EXECS)::
	scc -s -O -o $@ /usr/src/cmd/$@.c
	$(INS) -f /stand $@

iltd::
	cd iload; make -f iltd.mk install INS=$(INS)

rp6fmt::
	scc -s -o rp6fmt /usr/src/cmd/rp6fmt.c
rp6fmt::
	$(INS) -f /stand rp6fmt
fsdb::
	scc -s -o fsdb /usr/src/cmd/fsdb.c
fsdb::
	$(INS) -f /stand fsdb
mmtst:
	cd mmtest; make -f mmtest.mk INS=$(INS)
install:
	make -f pdp11.mk all INS=/etc/install

clean:
	rm -f *.o

clobber: clean
	rm -f $(EXECS) iltd rp6fmt fsdb
	cd iload; make -f iltd.mk clobber
	cd boots; make -f boots.mk clobber
	cd sash; make -f sash.mk clobber
	cd mmtest; make -f mmtest.mk clobber
