INS = :

all:	tapeboot diskboots boot2

tapeboot: tapeboot.o
	strip tapeboot.o
	dd if=tapeboot.o of=tmp1 count=1 conv=sync
	cat tmp1 tmp1 > tapeboot
	rm tmp1 tapeboot.o
	$(INS) -f /stand tapeboot

diskboots:
	ins=$(INS) sh -x boots.sh

boot2:
	as fs1boot.s
	strip a.out
	dd if=a.out of=boot1 bs=16 skip=1
	rm a.out
	as -o boot2 fs2boot.s
	strip boot2
	$(INS) -f /stand boot1
	$(INS) -f /stand boot2

clobber:
	rm -f tapeboot boot1 boot2
	rm -f rp03boot rp04boot rk11boot rf11boot rs04boot rl11boot
