INS = :

sash:	sash0.o sash1.o
	ld -o sash sash0.o sash1.o -l2
	rm -r sash0.o sash1.o
	$(INS) -f /stand sash

sash1.o:	sash1.c
	scc -c sash1.c

clobber:
	rm -f sash
