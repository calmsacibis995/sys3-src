#
/*
 *
 *	UNIX debugger
 *
 */

#include "defs.h"
#include "a.out.h"
SCCSID(@(#)setup.c	2.9);


MSG		BADNAM;
MSG		BADMAGIC;

MAP		txtmap;
MAP		datmap;
SYMSLAVE	*symvec;
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

#ifndef EDDT
readl(f,p,n) int f,n; long * p;{
#ifndef vax
	int t=0;
	do {t += read(f,&(p->loword),2); t += read(f,&(p->hiword),2); p++;} while (--n);
	return(t);
#else
	return(read(f,p,n*sizeof(long)));
#endif
}
#endif

setsym()
{
#ifndef EDDT
	INT		relflg;
	INT		symval, symflg;
	SYMSLAVE	*symptr;
	SYMPTR		symp;
	struct exec	txthdr;

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
			txtmap.b1=(magic==0411? 0x200:0);
			txtmap.e1=(magic==0407? symbas:
			           magic==0411? txtsiz+TXTHDRSIZ:
			           txtsiz);
			txtmap.f1=(magic==0411? 0x200:TXTHDRSIZ);
			txtmap.b2=datbas=(magic==0410? round(txtsiz,TXTRNDSIZ):
			                  magic==0411? txtmap.e1:
			                  0);
			txtmap.e2=txtmap.b2+(magic==0407?symbas:datsiz);
			txtmap.f2=(magic==0407? TXTHDRSIZ:
			           magic==0411? txtmap.e1:
			           TXTHDRSIZ+txtmap.e1);
			entrypt=txthdr.a_entry;
	/*		relflg=txthdr.a_drsize;
	/*		IF relflg!=1 THEN symbas =<< 1; FI
	*/
			symbas += txthdr.a_trsize+txthdr.a_drsize;
			symbas += TXTHDRSIZ;

			/* set up symvec */
			symvec=sbrk(shorten((1+symnum))*sizeof (SYMSLAVE));
			IF (symptr=symvec)==-1
			THEN	printf("%s\n",BADNAM);
				symptr=symvec=sbrk(sizeof (SYMSLAVE));
			ELSE	symset();
				WHILE (symp=symget()) ANDF errflg==0
				DO 
				    symflg=symp->symf;
				    symptr->valslave=symp->symv;
				    symptr->typslave=SYMTYPE(symflg);
				    IF (symflg & STABTYPES) != 0
				    THEN
					symptr->typslave=XSYM;
				    FI
				    symptr++;
				OD
			FI
			symptr->typslave=ESYM;
		FI
	FI
	IF magic==0 THEN txtmap.e1=maxfile; FI
#endif
}

setcor()
{
#ifndef EDDT
	fcor=getfile(corfil,2);
	datmap.ufd=fcor;
	IF read(fcor, &u, ctob(UPAGES))==ctob(UPAGES)
	   ANDF (u.u_pcb.pcb_ksp & 0xF0000000L)==0x80000000L
	   ANDF (u.u_pcb.pcb_usp & 0xF0000000L)==0x70000000L
	THEN	signo = u.u_arg[0]&017;
		txtsiz = ctob(u.u_tsize);
		datsiz = ctob(u.u_dsize);
		stksiz = ctob(u.u_ssize);
		datmap.b1=datbas=
			((magic==0410 || magic==0411)? round(txtsiz,TXTRNDSIZ):0);
		datmap.e1=(magic==0407?txtsiz:datmap.b1)+datsiz;
		datmap.f1 = ctob(USIZE);
		datmap.b2 = maxstor-stksiz;
		datmap.e2 = maxstor;
		datmap.f2 = ctob(USIZE)+
			((magic==0410 || magic==0411)?datsiz:datmap.e1);
		signo = *(ADDR *)(((ADDR)&u)+ctob(USIZE)-4*sizeof(int));
		IF magic ANDF magic!=u.u_exdata.ux_mag
		THEN	printf("%s\n",BADMAGIC);
		FI
	ELSE	/*
		** the following is in case (as on the vax) the address
		** space is larger than adb thinks a file can be
		*/
		datmap.e1 = (maxstor > maxfile) ? maxstor : maxfile;
	FI
#endif
}

#ifndef EDDT
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
#endif
