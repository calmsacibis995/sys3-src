/*
 * Retrieve files from GCOS via Spider or dataphone
 */

#include	<errno.h>
#include	<setjmp.h>

char	dpd[]	= "/usr/spool/dpd";
char	lock[]	= "/usr/spool/dpd/glock";
char	error[]	= "/usr/spool/dpd/errors";

#include	"daemon0.c"

#define MINUTES 60

#define OKTRB 0
#define BADTRB 1
#define FATTRB 2

char	zfname[]	= "/usr/spool/dpd/zf";
char	xfname[]	= "/usr/spool/dpd/xfXXXXXX";
char	badbuf[]	= "/usr/spool/dpd/fget.badbuf";
char	okmes[] = "OK nd =%3d, nf =%3d, nx = %-10d";
char	*backup[] = {"/usr/lib/fget.odemon", "/etc/fget.odemon", 0};
char	*argv1;

FILE	*ofd = NULL;		/*data file*/
FILE	*tfd = NULL;		/*badbuf file*/
int	tlaps = 0;
int	tlimit = 6*MINUTES;
int	tfac = 1;
int	nbaderr = 0;
int	gotit = 0;
int	ndelim = 0;
int	nfiles = 0;
int	nexec = 0;
int	execsw = 0;
long	nnull = 0;		/*count of consecutive nulls*/
char	fname[108];
jmp_buf	env;

int	(*output)();


main(argc, argv)

int argc;
char *argv[];

{

int i;
extern	ignore();

	dem_setup();

	if (argc == 2) {
		argv1 = argv[1];
		tlimit = 0;
		i = 0;
		while ( argv[1][i] ) tlimit = 10*tlimit +  argv[1][i++] - '0' ;
		}

#ifdef	DEBUG
	fprintf(stderr, "tlimit = %d\n",tlimit); /* PRINTF */
#endif

	setjmp(env);

	ofd = NULL;
	output = ignore;

	fgd_read();	/*perform input appropriate to device.*/

	if (gotit == 0)
		trouble(OKTRB, -30, "no output");

	if (gotit == 1) {
		gotit = 2;
		trouble(OKTRB, 30, okmes, ndelim, nfiles, nexec);
		}

	trouble(OKTRB, 0, okmes, ndelim, nfiles, nexec);
}



/* VARARGS */
trouble(lev, t, s, a1, a2, a3)
int	lev;
int	t;
char	*s;
int	a1, a2, a3;
{
	int i;

#ifdef	DEBUG
	fprintf(stderr, s, a1, a2, a3); putc('\n', stderr); /* PRINTF */
#endif
	alarm(0);
	dem_dis();	/*close appropriate device.*/
	if ( lev >= BADTRB ) nbaderr++;

	if ( lev <= BADTRB && nbaderr < 2 && t != 0 && tlaps < tlimit ) {
		if (t < 0) t = -t;
		t *= tfac;
		if (t > 4*MINUTES) t = 4*MINUTES;
#ifdef	DEBUG
		fprintf(stderr, "holding, t = %d, tlaps =  %d\n",t,tlaps); /* PRINTF */
#endif
		sleep(t);
		tlaps += t;
		tfac += tfac;
		if(access(lock, 0) == 0)
			longjmp(env, 1);
		logerr("glock has disappeared");
	}

	if(gotit)
		logerr(okmes, ndelim, nfiles, nexec);
	if(t >= 0 && lev > OKTRB)
		logerr(s, a1, a2, a3);

	if ( lev >= BADTRB ) {
		sleep(20);
		unlink(lock);
#ifndef DEBUG
		for(i=0; backup[i]; i++)
			execl(backup[i], backup[i], argv1, 0);
#endif
		}

	unlink(lock);
	exit(0);
}




ignore(b, i)
{
}


outputa(b, i)
char	*b;
int	i;
{
	for ( ; i -- > 0; b++ ) {
		if (*b == '\0') nnull++;
		else {
			for ( ; nnull > 0; nnull -- ) putc( '\0', ofd );
			putc( *b, ofd );
			}
		}
}


outputb(b, i)
char	*b;
int	i;
{
	for ( ; i-- > 0; b++ ) putc( *b, ofd );
}


firstln(b, i)
char	b[];
int	i;
{
	char	*fp, *sp, *tp;
	struct passwd *pwp, *getpwnam();
	extern	ignore(), outputa(), outputb();
	char *mktemp();

	output = ignore;
	execsw = 0;
	if ( i == 0 ) return;
	b[i-1] = '\0';
	if (i < 104 || b[1] != ' ') {
		logerr("illeg contr line  %s", b);
		return;
		}

	switch (b[0]) {

		case 'a':
		case 'A':
			output = outputa;
			break;

		case 'b':
		case 'B':
			output = outputb;
			break;

		case'x':
		case'X':
			execsw = 1;
			output = outputa;
			break;
		default:
			logerr("illegal control char - %s\n", b);
			return;

		}

	if(execsw == 0){
		fp = fname;
		sp = &b[2];
		if(*sp != '/'){
			while(*++sp != '/' && *sp) ;
			tp = dpd;
			if(*sp == 0){
				sp = &b[2];
				logerr("no directory specified for file %s", sp);
			}else{
				*sp++ = 0;
				if((pwp = getpwnam(&b[2])) != (struct passwd *)0)
					tp = pwp->pw_dir;
				else
					logerr("no login found for %s", &b[2]);
			}
			while(*tp)
				*fp++ = *tp++;
			*fp++ = '/';
		}
		while (*fp++ = *sp++);
	}else{
		strcpy(fname, xfname);
		tp = mktemp(fname);
		if(*++tp == 0){
			if(sizeof(backup) >= 3*sizeof(char *)){
				backup[0] = "/usr/lib/fget.demon";
				backup[1] = "/etc/fget.demon";
				backup[2] = 0;
			}
			trouble(BADTRB, 0, "Out of temporary files %s", fname);
		}
	}

#ifdef	DEBUG
	fprintf(stderr, "file = %s\n",fname); /* PRINTF */
#endif

	if ( (ofd = fopen(fname, "w")) == NULL) {
		output = ignore;
		logerr("cannot open: %s",  fname);
		}

	nnull = 0;
}



endfile()
{
int	n;
char	*fp;
extern	firstln();

	gotit = 1;
	ndelim++;
	output = firstln;
	if (ofd == NULL) return;
	FCLOSE(ofd);
	nnull = 0;

	if(execsw){
		execsw = 0;
		if(chmod(fname, 0777)){
			logerr("Can't chmod(%s).", fname);
			return;
		}
		if(n = system(fname)){
			logerr("system(%s) returned %o.", fname, n);
			return;
		}
		nexec++;
	}else{
		nfiles++;
		n = 0;
		fp = fname;
		while (n  < sizeof(zfname)-1)
			if (*fp++ != zfname[n++])
				return;
	}

	unlink(fname);


}
