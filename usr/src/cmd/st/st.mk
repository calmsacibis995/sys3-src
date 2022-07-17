CC = cc

CFLAGS = -O
LDFLAGS = -s

all:	stcntrl stload stproto

install:
	make -f st.mk $(ARGS)

stcntrl:	stcntrl.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o stcntrl stcntrl.c
	cp stcntrl /etc/stcntrl
	-chmod 775 /etc/stcntrl

stload::
	cp stload /etc/stload
	-chmod 775 /etc/stload

stproto:	mod40.r
	vpmc -m -o stproto mod40.r
	cp stproto /etc/stproto
	chmod 664 /etc/stproto

clean:
	-rm -f stcntrl stproto

clobber:	clean
