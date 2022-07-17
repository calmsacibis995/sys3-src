#include	<stdio.h>

/*
 *  fget -- spooler to get file from GCOS
 */

#define	SPTYPE	FGET
#define FIRSTCHAR 't'-1
#define	NTEMP	3
char	Xfname[NTEMP][25];
char	Xchar[NTEMP] = {'d', 't', 'z'};
char	*dfname = Xfname[0];
char	*tfname = Xfname[1];
char	*zfname = Xfname[2];
char	*NAME;

#include	"spool.c"
#include	"gcos.h"

char	fget_id[3] = FGET_ID;	/*FGET_ID must be defined in header file gcos.h*/
char	*fzf;
FILE	*pfdes;
char	wdir[BUFSIZ];
FILE	*uff;
char	fname[CARDSIZE]  /* unix file name */;
char	*fmode;

main(agc, agv)
int agc;
char *agv[];
{
	extern FILE *popen();
	int fi, i, j, nslash, c;
	int time;
	int poff, foff;

	argc = agc;    
	argv = agv;
	setup();
	remotsw = TOSS;		/*default is toss online output*/
#ifndef NMAIL
	wantmail = 1;		/*default is send mail.*/
#endif
	grade = '1';
	fmode = "a";
	fi = 0;
	foff = 0;
	poff = 0;

	while (argc>1 && (arg = argv[1])[0]=='-') {
		if(!comopt(arg[1]))
			switch (arg[1]) {

			case 'a':
				fmode = "a";
				break;

			case 'b':
				fmode = "b";
				break;

			case 'd':
				arg = getarg('d');
				while((fname[fi] = arg[fi])) ++fi;
				fname[fi++] = '/';
				break;

			case 'u':
				arg = getarg('u');
				i = 0;
				while((gcosid[i] = arg[i]))
					if(++i > 12) {
						gcosid[i] = '\0';
						break;
					}
				uidf = 1;
				break;

			default:
				err("Unrecognized option: %s", arg);
				break;
			}
		argc--;
		argv++;
	}

	gcos1();		/*setup initial GCOS cards.*/
	card('L',"$	select	cc/unixsupport/access");
	if(gcdebug)
		card('L',"$	select	cc/test/fget");
	else
		card('L',"$	select	cc/unixsupport/fget");
	card('L',fget_id);
	while(--argc) {
		arg = *++argv;
		if(mailfile == 0)
			mailfile = arg;
		/* extract a unix file name */

		i = 0;
		foff = fi;
		nslash = 0;
		while((c = argv[0][i++]) != '\0') {
			if(c == '/') { 
				nslash++; 
				foff = fi; 
				continue; 
			}
			fname[foff++] = c;
		}
		fname[foff++] = '\0';
		fname[fi+12] = '\0';

		/* create the ultimate output file */

		if(debug == 0){
			if((uff = fopen(fname, "w")) == NULL) {
				err("Cannot create %s",fname);
				continue;
			}
			chmod(fname,0666);	/*insure file writeable by daemon. MRW*/
		}

		/* now make a link for the dpd */

		if(zfname[INCHAR] < 'z')    /*don't create file names past 'z'. MRW*/
			zfname[INCHAR]++;
		fzf = zfname;
		if(link(fname, fzf) != 0){
			if( fi > 0 ) { 
				fzf = fname; 
			}

			else{
				if(poff == 0){
					if((pfdes = popen("pwd", "r")) != NULL){
						int c;
						while ((c = getc(pfdes)) != EOF)
							wdir[poff++] = c;
						pclose(pfdes);
						wdir[poff-1] = '/';
					}
				}
				if(wdir[0] != '/')  
					errout("Cannot find directory pathname");
				i = poff; 
				j = 0;
				while ( wdir[i++] = fname[j++]);
				if(link(wdir, fzf) != 0) fzf = wdir;
			}
		}

		sprintf(cardbuf, "%s %s%s%s %s",
				fmode,
				(nslash == 0 || *arg == '/') ? gcosid : "",
				(nslash == 0) ? "/" : "",
				arg,
				fzf );
		card('L', cardbuf);

		if(debug == 0){
			i = 2;
			while((putc(cardbuf[i++], uff)) != '\0') ;
			c = putc( '\n', uff );
				if(c == EOF && ferror(uff)){
					err("Error writing %s", fname);
					break;
				}
			fclose(uff);
		}
		nact++;

	}

	fflush(tff);
	if(fork() != 0){
		gcos2();		/*put out final cards, and spool job.*/
	}

	signal(SIGHUP, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	dfname[INCHAR]++;
	for( time = 4 ; time < 200 ; time += time ) {
		sleep(time);
		if(access(dfname, 0) != 0) break;
	}

	execl("/usr/lib/fget.demon", "fget.demon", 0);
	execl("/etc/fget.demon", "fget.demon", 0);
	err("Execl of fget.demon failed.");
	exit(1);
}
