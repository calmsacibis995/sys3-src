FFLAG =

CC = cc

CFLAGS = -O

install:
	-if vax; then cd vax; \
		make -f crash.mk ARGS="$(ARGS)" CFLAGS="$(CFLAGS)" \
		FFLAG="$(FFLAG)" install clobber; \
		else cd pdp11; \
		make -f crash.mk ARGS="$(ARGS)" CFLAGS="$(CFLAGS)" \
		FFLAG="$(FFLAG)" install clobber; fi

clobber:
