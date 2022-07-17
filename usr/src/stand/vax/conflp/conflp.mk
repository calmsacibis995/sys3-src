all:	sash
	cp update /stand/conflp
	cp *.cmd /stand/conflp
	cp sash /stand/conflp
	rm sash

sash:
	/stand/vrmhead /stand/sash sash
