#
/*
 *
 *	UNIX debugger
 *
 */

#include "head.h"
#include <a.out.h>

MSG		BADMAGIC;

INT		wtflag;
INT		fcor;
INT		fsym;
L_INT		maxfile;
L_INT		maxstor;
L_INT		txtsiz;
L_INT		datsiz;
L_INT		datbas;
L_INT		stksiz;
STRING		errflg;
INT		magic;
L_INT		symbas;
L_INT		symnum;
L_INT		entrypt;

INT		argcount;
INT		signo;
struct user u;

STRING		symfil	= "a.out";
STRING		corfil	= "core";

#define TXTHDRSIZ	(sizeof(txthdr))

readl(f,p,n) int f,n; long * p;{
#ifndef vax
	int t=0;
	do {t += read(f,&(p->loword),2); t += read(f,&(p->hiword),2); p++;} while (--n);
	return(t);
#else
	return(read(f,p,n*sizeof(long)));
#endif
}

setsym()
{
	struct exec txthdr;	/* a.out header segment */

	fsym=getfile(symfil,1);
	txtmap.ufd=fsym;
	IF read(fsym, &txthdr, sizeof(txthdr))==TXTHDRSIZ
	THEN	magic=txthdr.a_magic;
		IF magic!=0411 ANDF magic!=0410 ANDF magic!=0407 ANDF magic!=0405
		THEN	magic=0;
		ELSE	symnum=txthdr.a_syms/SYMTABSIZ;
			txtsiz=txthdr.a_text;
			datsiz=txthdr.a_data;
			symbas=txtsiz+datsiz;
#ifndef VAX135
			txtmap.b1=0;
			txtmap.e1=(magic==0407?symbas:txtsiz);
			txtmap.f1 = TXTHDRSIZ;
			txtmap.b2=datbas=(magic==0410?round(txtsiz,TXTRNDSIZ):0);
#else
			txtmap.b1=(magic==0411? 0x200:0);
			txtmap.e1=(magic==0407? symbas:
			           magic==0411? txtsiz+TXTHDRSIZ:
			           txtsiz);
			txtmap.f1=(magic==0411? 0x200:TXTHDRSIZ);
			txtmap.b2=datbas=(magic==0410? round(txtsiz,TXTRNDSIZ):
			                  magic==0411? txtmap.e1:
			                  0);
#endif
			txtmap.e2=txtmap.b2+(magic==0407?symbas:datsiz);
#ifndef VAX135
			txtmap.f2 = TXTHDRSIZ+(magic==0407?0:txtmap.e1);
#else
			txtmap.f2=(magic==0407? TXTHDRSIZ:
			           magic==0411? txtmap.e1:
			           TXTHDRSIZ+txtmap.e1);
#endif

			entrypt=txthdr.a_entry;
			symbas += txthdr.a_trsize+txthdr.a_drsize;
			symbas += TXTHDRSIZ;
			ststart = symbas;

			/* set up symvec */
		FI
	FI
	IF magic==0 THEN txtmap.e1=maxfile; FI
}

setcor()
{
	fcor=getfile(corfil,2);
	datmap.ufd=fcor;
	IF read(fcor, &u, ctob(UPAGES))==ctob(UPAGES)
#ifdef VAX135
	   ANDF (u.u_pcb.pcb_ksp & 0xF0000000L)==0x70000000L
#else
	   ANDF (u.u_pcb.pcb_ksp & 0xF0000000L)==0x80000000L
#endif
	   ANDF (u.u_pcb.pcb_usp & 0xF0000000L)==0x70000000L
	THEN	
		signo = u.u_arg[0]&017;
		txtsiz = ctob(u.u_tsize);
		datsiz = ctob(u.u_dsize);
		stksiz = ctob(u.u_ssize);
#ifndef VAX135
		datmap.b1 = datbas = (magic==0410?round(txtsiz,TXTRNDSIZ):0);
#else
		datmap.b1=datbas=
			((magic==0410 || magic==0411)? round(txtsiz,TXTRNDSIZ):0);
#endif
		datmap.e1=(magic==0407?txtsiz:datmap.b1)+datsiz;
#ifdef STD
		datmap.f1 = ctob(USIZE);
#else
		datmap.f1 = ctob(UPAGES);
#endif
		datmap.b2 = maxstor-stksiz;
		datmap.e2 = maxstor;
#ifdef STD
		datmap.f2 = ctob(USIZE)+(magic==0410?datsiz:datmap.e1);
#else
#ifndef VAX135
		datmap.f2 = ctob(UPAGES)+(magic==0410?datsiz:datmap.e1);
#else
		datmap.f2 = ctob(UPAGES)+
			((magic==0410 || magic==0411)?datsiz:datmap.e1);
#endif
#endif
		signo = *(ADDR *)(((ADDR)&u)+ctob(UPAGES)-4*4);
		IF magic ANDF magic!=u.u_exdata.ux_mag
		THEN	printf("%s\n",BADMAGIC);
		FI
	ELSE	datmap.e1 = maxfile;
	FI
}

create(f)
STRING	f;
{	int fd;
	IF (fd=creat(f,0644))>=0
	THEN close(fd); return(open(f,wtflag));
	ELSE return(-1);
	FI
}

getfile(filnam,cnt)
STRING	filnam;
{
	REG INT		fsym;

	IF !eqstr("-",filnam)
	THEN	fsym=open(filnam,wtflag);
		IF fsym<0 ANDF argcount>cnt
		THEN	IF wtflag
			THEN	fsym=create(filnam);
			FI
			IF fsym<0
			THEN printf("cannot open `%s'\n", filnam);
			FI
		FI
	ELSE	fsym = -1;
	FI
	return(fsym);
}
