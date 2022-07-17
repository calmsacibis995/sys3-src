/*
 * cc command -- calls the appropriate passes of the
 *		c compiler, assembler, and loader.
 */

char	release[] = "@(#)C rel 2.3; UTS rel 1.3.";

# include <stdio.h>
# include <ctype.h>
# include <signal.h>

#ifndef CSW
#include "*****ERROR: CSW undefined*****"
#endif

#define	DMR	0
#define	SCJ1	1
#define	SCJ2	2
#define	JFR	3
#define	SCPTR	sizeof(char *)

#define	Xc0	0
#define	Xc1	1
#define	Xc2	2
#define	Xcp	3
#define	Xas	4
#define	Xld	5
#define	NPASS	6

#if CSW == DMR
#define	NAMEc0 "c0"
#define	NAMEc1 "c1"
#endif
#if CSW == SCJ1
#define	NAMEc0 "comp"
#define	NAMEc1 0
#endif
#if CSW == SCJ2
#define	NAMEc0 "front"
#define	NAMEc1 "back"
#endif
#if CSW == JFR
#define	NAMEc0 "ccom"
#define	NAMEc1 0
#endif
#define	NAMEc2 "c2"
#define	NAMEcp "cpp"
#define	NAMEas "as"
#define	NAMEld "ld"

char	*tmp0;
char	*tmp1;
char	*tmp2;
char	*tmp3;
char	*tmp4;
char	*tmp5;
char	*outfile;
char	**clist;
char	**llist;
char	**av;
char	*alist;
int	dflag;
int	pflag;
int	sflag;
int	cflag;
int	eflag;
int	gflag;
int	exflag;
int	oflag;
int	proflag;
int	noflflag;
int	zflag;		/* request zero-extension on all chars */
int	exfail;
char	fchar;
char	bflag;

#ifndef CCNAME
#define	CCNAME	"cc"
#endif
char	*chpass ;
char	*npassname ;
char	rpassname[] = "/lib//";
char	alpassname[] = "/bin/";
char	*nameps[NPASS] = {NAMEc0, NAMEc1, NAMEc2, NAMEcp, NAMEas, NAMEld};
char	*passes[NPASS];
char	altpass[NPASS];
char	*pref = "/lib/crt0.o";
extern	int optind;
extern	int opterr;
extern	char *optarg;
extern	int optopt;

char	*copy(), *setsuf(), *stralloc();
char	*strcat(), *strncat();
char	*strncpy(), *strcpy(), *strrchr();
char	*mktemp(), *malloc();

main(argc, argv)
char *argv[]; 
{
	char *t;
	char *assource;
	char **pv, **ptemp, **pvt;
	int nc, nl, i, j, nargs, c, f20, nxo, na;
	int idexit();

	opterr = 0;
	i = nc = nl = f20 = nxo = 0;
	nargs = argc + 1;
	j = SCPTR * nargs;
	if((llist = (char **)stralloc(j)) != (char **)0)
	  if((clist = (char **)stralloc(j)) != (char **)0)
	    if((av = (char **)stralloc(j + 5 * SCPTR)) != (char **)0)
	      if((ptemp = (char **)stralloc(j)) != (char **)0)
		;
	else{
		error("%s: Not enough space to set up tables.\n", argv[0]);
		exit(1);
	}
	pv = ptemp;
	setbuf(stdout, (char *)NULL);
	if((t = strrchr(argv[0], '/')) == NULL)
		fchar = *argv[0];
	else
		fchar = t[1];
	if(fchar == CCNAME[0])
		fchar = 0;
	while (optind<argc)
	  switch (c = getopt(argc, argv, "So:OpEPcfD:I:U:Ct:B:zgd:2l:V:")) {
	case 'S':
		sflag++;
		cflag++;
		continue;

	case 'o':
		outfile = optarg;
		if ((c=getsuf(outfile))=='c'||c=='o') {
			error("Would overwrite %s", outfile);
			exit(8);
		}
		continue;

	case 'O':
		oflag++;
		continue;

	case 'p':
		proflag++;
		continue;

	case 'g':
		gflag++;
		continue;

	case 'c':
		cflag++;
		continue;

	case 'f':
#if vax
		fprintf(stderr, "'-f' option ignored on VAX\n");
#else
		noflflag++;
#endif
		continue;

	case '2':
		pref = "/lib/crt2.o";
		f20 = 1;
		continue;

	case 'E':
		exflag++;
	case 'P':
		pflag++;
		cflag++;
	case 'D':
	case 'I':
	case 'U':
	case 'C':
		if (pv >= ptemp+nargs) {
			error("Too many EPDIUC options", (char *)NULL);
			continue;
		}
		*pv = stralloc(strlen(optarg)+2);
		(*pv)[0] = '-';
		(*pv)[1] = c;
		(*pv)[2] = '\0';
		strcat(*pv, optarg);
		pv++;
		continue;

	case 't':
		if (chpass)
			error("-t overwrites earlier option", (char *)NULL);
		chpass = optarg;
		if (chpass[0]==0)
			chpass = "012p";
		continue;

	case 'B':
		if (npassname)
			error("-B overwrites earlier option", (char *)NULL);
		npassname = optarg;
		if (npassname[0]==0){
			npassname = "/lib/o";
			bflag = 0;
		}else
			bflag = 1;
		continue;

	case 'd':
		dflag++;
		alist = stralloc(strlen(optarg)+2);
		strcpy(alist, "-d");
		strcat(alist, optarg);
		continue;

	case 'z':
		zflag++;
		continue;

	case 'l':
	case 'V':
		t = stralloc(strlen(optarg)+2);
		t[0] = '-';
		t[1] = c;
		t[2] = '\0';
		strcat(t, optarg);
		goto checknl;
	case '?':
		t = stralloc(3);
		t[0] = '-';
		t[1] = optopt;
		t[2] = '\0';
checknl:
		if(nl >= nargs){
			free(t);
			error("Too many ld options", (char *)NULL);
			continue;
		}
		llist[nl++] = t;
		continue;

	case EOF:
		t = argv[optind];
		optind++;
		if((c=getsuf(t))=='c' || c=='s'|| exflag) {
			clist[nc++] = t;
			t = setsuf(t, 'o');
		}
		if (nodup(llist, t)) {
			if(nl >= nargs){
				error("Too many ld arguments", (char *)NULL);
				continue;
			}
			llist[nl++] = t;
			if (getsuf(t)=='o')
				nxo++;
		}
	}
	if(eflag)
		exit(eflag);
	if (gflag)
		oflag = 0;
	if(fchar) {
		(*pv++ = "-I/usr/Xinclude")[7] = fchar;
	}
	if (npassname && chpass ==0)
		chpass = "012p";
	if (chpass && npassname==0)
		npassname = "/lib/n";
	if (chpass)
		for (t=chpass; *t; t++) {
			switch (*t) {
			default:
				continue;
			case '0':
			case '1':
			case '2':
				i = *t - '0';
				break;
			case 'p':
				i = Xcp;
				break;
			case 'a':
				i = Xas;
				break;
			case 'l':
				i = Xld;
				break;
			}
			mkpname (i, npassname);
			altpass[i] = !bflag;
			continue;
		}
	rpassname[sizeof(rpassname)-2] = fchar;
	for (i = 0; i < NPASS; i++)
		if(passes[i] == 0){
			mkpname (i, i < Xas ? rpassname : alpassname);
			altpass[i] = 1;
		}
	if (noflflag)
		pref = proflag ? "/lib/fmcrt0.o" : "/lib/fcrt0.o";
	else if (proflag)
		pref = "/lib/mcrt0.o";
	if(nc==0)
		goto nocom;
	if (pflag==0) {
		tmp0 = mktemp("/tmp/ctm0XXXXXX");
		if((creat(tmp0, 0400))<0) {
			error("cc: cannot create temp", (char *)NULL);
			exit(eflag);
		}
	}
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, idexit);
	if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
		signal(SIGTERM, idexit);
	(tmp1 = copy(tmp0))[8] = '1';
	(tmp2 = copy(tmp0))[8] = '2';
	(tmp3 = copy(tmp0))[8] = '3';
	if (oflag)
		(tmp5 = copy(tmp0))[8] = '5';
	if (pflag==0)
		(tmp4 = copy(tmp0))[8] = '4';
	pvt = pv;
	for (i=0; i<nc; i++) {
		if (nc>1)
			printf("%s:\n", clist[i]);
		if (getsuf(clist[i])=='s') {
			assource = clist[i];
			goto assemble;
		} 
		else
			assource = tmp3;
		if (pflag)
			tmp4 = setsuf(clist[i], 'i');
		av[0] = nameps[Xcp];
		av[1] = clist[i];
		av[2] = exflag ? "-" : tmp4;
		na = 3;
		for(pv=ptemp; pv <pvt; pv++)
			av[na++] = *pv;
		av[na]=0;
		if (callsys(passes[Xcp], av, &altpass[Xcp])) {
			exfail++; 
			eflag++;
		}
		if (pflag || exfail) {
			cflag++;
			continue;
		}
		j = 0;
		av[j++] = nameps[Xc0];
		if(zflag)
			av[j++] = "-u";
		av[j++] = tmp4;
		if (nameps[Xc1] == 0){
			if (sflag)
				assource = tmp3 = setsuf(clist[i], 's');
			av[j++] = tmp3;
			if (oflag)
				av[j-1] = tmp5;
		}else
			av[j++] = tmp1;
#if CSW == DMR
		av[j++] = tmp2;
#endif
		if (proflag) {
#if CSW == DMR
			av[j++] = "-P";
#else
			av[j++] = "-XP";
#endif
		} 
		if (gflag)
			av[j++] = "-Xg";
#if CSW == SCJ1  ||  CSW == SCJ2
		if(sflag)
			av[j++] = "-l";
#endif
		av[j] = 0;
		if (callsys(passes[Xc0], av, &altpass[Xc0])) {
			cflag++;
			eflag++;
			continue;
		}
		if(nameps[Xc1] != (char *)0){
			j = 0;
			av[j++] = nameps[Xc1];
			av[j++] = tmp1;
#if CSW == DMR
			av[j++] = tmp2;
#endif
			if (sflag)
				assource = tmp3 = setsuf(clist[i], 's');
			av[j++] = tmp3;
			if (oflag)
				av[j-1] = tmp5;
#if CSW == SCJ1  ||  CSW == SCJ2
			if(sflag)
				av[j++] = "-l";
#endif
			av[j] = 0;
			if(callsys(passes[Xc1], av, &altpass[Xc1])) {
				cflag++;
				eflag++;
				continue;
			}
		}
		if (oflag) {
			av[0] = nameps[Xc2];
			av[1] = tmp5;
			av[2] = tmp3;
			av[3] = 0;
			if (callsys(passes[Xc2], av, &altpass[Xc2])) {
				unlink(tmp3);
				assource = tmp5;
			} 
			else
				unlink(tmp5);
		}
		if (sflag)
			continue;
assemble:
		j = 0;
		av[j++] = nameps[Xas];
#if CSW != JFR
		av[j++] = "-u";
#endif
		av[j++] = "-o";
		av[j++] = setsuf(clist[i], 'o');
		av[j++] = assource;
		if (dflag)
			av[j++] = alist;
		av[j] = 0;
		cunlink(tmp1);
		cunlink(tmp2);
		cunlink(tmp4);
		if (callsys(passes[Xas], av, &altpass[Xas]) > 0) {
		  /*this test used to be "> 1" for old pdp11 as*/
			cflag++;
			eflag++;
			continue;
		}
	}
nocom:
	if (cflag==0 && nl!=0) {
		i = 0;
		av[0] = nameps[Xld];
		av[1] = "-X";
		av[2] = pref;
		j = 3;
		if (outfile) {
			av[j++] = "-o";
			av[j++] = outfile;
		}
		while(i<nl)
			av[j++] = llist[i++];
		if (gflag)
			av[j++] = "-lg";
		if(f20)
			av[j++] = "-l2";
		else {
			if(fchar) {
				(av[j++] = "-lXc")[2] = fchar;
			}
			else {
				av[j++] = "-lc";
			}
		}
		av[j] = 0;
		eflag |= callsys(passes[Xld], av, &altpass[Xld]);
		if (nc==1 && nxo==1 && eflag==0)
			cunlink(setsuf(clist[0], 'o'));
	}
	dexit();
}

idexit()
{
	eflag = 100;
	dexit();
}

dexit()
{
	if (!pflag) {
		cunlink(tmp1);
		cunlink(tmp2);
		if (sflag==0)
			cunlink(tmp3);
		cunlink(tmp4);
		cunlink(tmp5);
		cunlink(tmp0);
	}
	exit(eflag);
}

error(s, x)
char *s, *x;
{
	fprintf(stderr, s, x);
	putc('\n', stderr);
	exfail++;
	cflag++;
	eflag++;
}




getsuf(as)
char as[];
{
	register int c;
	register char *s;
	register int t;

	s = as;
	c = 0;
	while(t = *s++)
		if (t=='/')
			c = 0;
		else
			c++;
	s -= 3;
	if (c<=14 && c>2 && *s++=='.')
		return(*s);
	return(0);
}

char *
setsuf(as, ch)
char *as;
{
	register char *s, *s1;

	s = s1 = copy(as);
	while(*s)
		if (*s++ == '/')
			s1 = s;
	s[-1] = ch;
	return(s1);
}

callsys(f, v, alt)
char f[], *v[]; 
char *alt;
{
	int t, status;

	if ((t=fork())==0) {
		execv(f, v);
		if(*alt != 0){
			alt = stralloc(4 + strlen(f));
			strcpy(alt, "/usr");
			strcat(alt, f);
			execv(alt, v);
		}
		fprintf(stderr, "Can't find %s, %s\n", f, alt);
		exit(100);
	} else
		if (t == -1) {
			fprintf(stderr, "Try again\n");
			return(100);
		}
	while(t!=wait(&status))
		;
	if (t = status&0377) {
		if (t!=SIGINT) {
			fprintf(stderr, "Fatal error in %s\n", f);
			eflag = 8;
		}
		dexit();
	}
	return((status>>8) & 0377);
}

char *
copy(s)
register char *s;
{
	register char *ns;

	ns = stralloc(strlen(s));
	return(strcpy(ns, s));
}

char *
stralloc(n)
int	n;
{
	char *malloc();
	register char *s;

	s = malloc(n+1);
	if (s==NULL) {
		error("out of space", (char *)NULL);
		dexit();
	}
	return(s);
}


nodup(l, os)
char **l, *os;
{
	register char *t, *s;
	register int c;

	s = os;
	if (getsuf(s) != 'o')
		return(1);
	while(t = *l++) {
		while(c = *s++)
			if (c != *t++)
				break;
		if (*t=='\0' && c=='\0')
			return(0);
		s = os;
	}
	return(1);
}

cunlink(f)
char *f;
{
	if (f==NULL)
		return;
	unlink(f);
}


mkpname(n, pname)
int	n;
char	*pname;
{
	if (nameps[n] == 0)
		return;
	passes[n] = stralloc(strlen(pname) + strlen(nameps[n]));
	strcpy(passes[n], pname);
	strcat(passes[n], nameps[n]);
}
