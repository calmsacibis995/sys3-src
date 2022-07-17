INS = :
all:	tapeboot rpboot

tapeboot::	tapeboot.o
	strip tapeboot.o
	dd if=tapeboot.o of=tmp1 bs=32 skip=1
	dd if=tmp1 of=tmp2 bs=1b conv=sync
	cat tmp2 tmp2 >tapeboot
	rm tmp1 tmp2 tapeboot.o
tapeboot::
	$(INS) -f /stand tapeboot

rpboot::	rpboot.o
	strip rpboot.o
	dd if=rpboot.o of=tmp1 bs=32 skip=1
	dd if=tmp1 of=rpboot bs=1b conv=sync
	rm tmp1 rpboot.o
rpboot::
	$(INS) -f /stand rpboot
clean:
	rm -f *.o tmp*

clobber: clean
	rm -f tapeboot rpboot
