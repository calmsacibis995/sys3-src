CC = cc

CFLAGS = -O
FFLAG =
LDFLAGS = -s

all:	prfld prfdc prfpr prfsnap prfstat

install:
	make -f profiler.mk $(ARGS) FFLAG="$(FFLAG)" CFLAGS="$(CFLAGS)"


prfld:		prfld.c
	$(CC) $(CFLAGS) $(FFLAG) $(LDFLAGS) -o prfld prfld.c
	cp prfld /etc/prfld
	-chmod 775 /etc/prfld

prfdc:		prfdc.c
	$(CC) $(CFLAGS) $(FFLAG) $(LDFLAGS) -o prfdc prfdc.c
	cp prfdc /etc/prfdc
	-chmod 775 /etc/prfdc

prfpr:		prfpr.c
	$(CC) $(CFLAGS) $(FFLAG) $(LDFLAGS) -o prfpr prfpr.c
	cp prfpr /etc/prfpr
	-chmod 775 /etc/prfpr

prfsnap:	prfsnap.c
	$(CC) $(CFLAGS) $(FFLAG) $(LDFLAGS) -o prfsnap prfsnap.c
	cp prfsnap /etc/prfsnap
	-chmod 775 /etc/prfsnap

prfstat:	prfstat.c
	$(CC) $(CFLAGS) $(FFLAG) $(LDFLAGS) -o prfstat prfstat.c
	cp prfstat /etc/prfstat
	-chmod 775 /etc/prfstat

clean:
	-rm -f prfdc prfld prfpr prfsnap prfstat

clobber:	clean
