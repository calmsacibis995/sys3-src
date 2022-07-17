all:
	echo "\n standalone only\n"
install: all
clobber: clean
clean:
	rm -f ./rp6fmt
