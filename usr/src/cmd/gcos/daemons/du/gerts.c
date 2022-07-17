/* gerts communication subroutines*/

#include "gcos.h"
#include <stdio.h>
#include <signal.h>

/* The following parameters are now defined in "gcos.h". */
char	du[] = DEVDU;		/*dataphone device*/
char	dn[] = DEVDN;		/*ACU device*/
char	*numbers[] = NUMBERS;	/*phone numbers to dial GCOS*/

#define	FCLOSE(F)	if(F != -1){ close(F);  F = -1;}
#define	SCHAR(C)	buf[index++] = C;  bcc ^= C
#define	CCHAR(C)	if((C = buf[index++]) == 0) goto reject;  bcc ^= C

#define	NCONERR	4
#define	BUSY	-1
#define	TIMEOUT	-2
#define	BADID	-3
#define	ERROR	-4

#define	INITBUF	"\026\026\026\026\001\000\000\100\000\100"
#define	BCCINIT	0
#define	INDEXFC	5
#define	LBUF 400

#define	ACKOC	0102
#define	SOH	001
#define	STX	002
#define	ETX	003
#define	SYNC	026

char	idmes[] = "H$*$XX passwd\036\003";
char	dis[]	= "H$*$dis\036\003";
int	duio = -1;
int	dno = -1;
int	gclosed = 1;		/*0 means must send GRTS a disconnect.*/
int	osc;
int	gertsrc;

connect(id)
char id[2];
{
	register int i;
	int j, n, ioc, saverc;
	int gertsto();

	signal(SIGALRM, gertsto);
	gertsrc = ERROR;
	idmes[4] = id[0];
	idmes[5] = id[1];
	FCLOSE(dno);		/*I hope this isn't necessary. MRW*/
	FCLOSE(duio);		/*ditto*/

	for(j = 0; numbers[j]; j++){
		if((duio = open(du, 2)) == -1)
			return(BUSY);
		if((dno = open(dn, 1)) == -1){
			FCLOSE(duio)
			return(BUSY);
		}
		i = strlen(numbers[j]);
		if((n = awrite(dno, numbers[j], i, 120)) != i)
			goto retry;
		FCLOSE(dno);
		gclosed = 0;
		sleep(10);
		osc = 0101;
		if ((ioc = gerts(0103, 0102, 0, 0)) != ACKOC)
			goto retry;
		if((ioc = gerts(0104, 0100, idmes, 0)) == ACKOC)
			return(0);
		gertsrc = BADID;
#if DEBUG >= 1
		fprintf(stderr, "ioc=%o\n", ioc);
#endif
retry:
		saverc = gertsrc;
		gerts_dis();
		if(numbers[j+1])
			sleep(20);
	}
	return(saverc);
}


gerts(ofc, ooc, ibuf, obuf)
char ofc, ooc;
char *ibuf, *obuf;
{
	int errcnt;
	char ifc, isc, ioc, bcc, dflg;
	char nextoc;
	static char buf[LBUF+1];
	register int index, i;
	register char c;
	int n;

	int gertsto();
	signal(SIGALRM, gertsto);
#if DEBUG >= 1
	fprintf(stderr, "gerts(%o, %o, %.20s, %.20s);\n", ofc, ooc, ibuf, obuf);
#endif
	gertsrc = ERROR;
	if(duio == -1)
		return(gertsrc);
	nextoc = ooc & 0707;

	for(errcnt = 0; errcnt < 4; errcnt++){
	
		for(i = 0; i< sizeof(INITBUF)-1; i++)
			buf[i] = INITBUF[i];
		bcc = BCCINIT;
		index = INDEXFC;
		SCHAR(ofc);
		SCHAR(osc);
		index++;
		SCHAR(nextoc);
		index = sizeof(INITBUF)-1;
		if(ofc == 0103){
			SCHAR(0111);
		}
		SCHAR(STX);
		if(ibuf)
			for(i = 0; ibuf[i] != ETX; i++){
				SCHAR(ibuf[i]);
				if(index >= LBUF-4)
					goto gerror;
			}
		SCHAR(ETX);
		SCHAR(bcc);
		SCHAR(SYNC);
		SCHAR(SYNC);
		if(index & 01){
			SCHAR(SYNC);
		}
		buf[index] = 0;
	
#if DEBUG >= 2
		fprintf(stderr, "write # %d('%s', %d)\n", errcnt, buf, index);
#endif
		if((n = awrite(duio, buf, index, 10)) != index)
			goto gerror;
		for(i=0; i<LBUF+1; i++)
			buf[i] = 0;
		if((n = aread(duio, buf, LBUF, 30)) <= 0)
			goto gerror;
#if DEBUG >= 2
		fprintf(stderr, "read # %d('%s', %d)\n", errcnt, buf, n);
#endif

		index = 0;
		c = SYNC;
		while(c == SYNC){
			CCHAR(c);
		}
		if(c != SOH)  goto reject;
		bcc = 0;
		CCHAR(ifc);
		CCHAR(isc);
		if(isc != 0101 && isc != 0102)
			goto reject;
		CCHAR(c);
		if(c != 0100)  goto reject;
		CCHAR(ioc);
		CCHAR(c);
		if(c != 0100)  goto reject;
		CCHAR(c);
		if(c != STX)  goto reject;
		dflg = 0;
		CCHAR(c);
		while(c != ETX){
			if(dflg == 0) dflg = index-1;
			CCHAR(c);
			if(index >= LBUF-1) goto reject;
		}
		if((buf[index] & 0177) != (bcc & 0177))  goto reject;
#if DEBUG >= 1
		buf[index+1] = 0;
		fprintf(stderr, "received fc=%o, oc=%o, message='%.40s'\n",
			ifc, ioc, dflg ? &buf[dflg] : &buf[index+1]);
#endif
		if(osc != isc)
			dflg = 0;
		if(((dflg == 0) && (ioc & 070)) || (ioc == 0106))
			goto accept;
		if(++osc >= 0103)
			osc = 0101;
		if(obuf){
			obuf[0] = ifc;
			i = 1;
			if(index = dflg)
				while(buf[index] != ETX)
					obuf[i++] = buf[index++];
			obuf[i] = ETX;
		}
		return(ioc & 0707);

accept:
		nextoc = ooc & 0707;
		continue;
reject:
#if DEBUG >= 2
		fprintf(stderr, "reject: index = %d\n", index);
#endif
		nextoc = ooc | 010;
	}
	if(gertsrc == ERROR)
		gertsrc = ERROR - index - 1;

gerror:
#if DEBUG >= 1
	fprintf(stderr, "gerror: n=%d, index=%d.\n  buf='%s'.\n",
		n, index, buf);
#endif
	return(gertsrc);
}


gerts_dis()
{
	if(gclosed++ <= 0)
		gerts(0104, 0100, dis, 0);
	FCLOSE(duio);
	FCLOSE(dno);
}


awrite(dev, buf, ct, time)
int dev, ct;
unsigned time;
char *buf;
{
	register int n;

	alarm(time);
	n = write(dev, buf, ct);
	alarm(0);
	return(n);
}


aread(dev, buf, ct, time)
int dev, ct;
unsigned time;
char *buf;
{
	register int n;

	alarm(time);
	n = read(dev, buf, ct);
	alarm(0);
	return(n);
}


gertsto()
{
	gertsrc = TIMEOUT;
}
