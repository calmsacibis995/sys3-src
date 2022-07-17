#include	<stdio.h>
#include	<sys/utsname.h>

/*
 *  gcosmail -- spooler for sending GCOS mail
 */

#define	SPTYPE	GCOSA
#define FIRSTCHAR 'm'-1
#define	NTEMP	4
char	Xfname[NTEMP][25];
char	Xchar[NTEMP] = {'d', 't', 'c', 'l'};
char	*dfname = Xfname[0];
char	*tfname = Xfname[1];
char	*cfname = Xfname[2];
char	*lfname = Xfname[3];
char	*NAME;
#define	BF	'B'
#define	GCATSW	0
#define	MAXCOPY	204800L

#include	"spool.c"

main(agc, agv)
int agc;
char *agv[];
{
	struct utsname utsname;
	long tb;

	argc = agc;    argv = agv;
	setup();
	remotsw = TOSS;
#ifndef NMAIL
	wantmail = 1;		/*default is send mail.*/
#endif
	mailfile = "pipe.end";
	grade = '1';

	while (argc>1 && (arg = argv[1])[0]=='-') {
		if(!comopt(arg[1]))
		switch (arg[1]) {

		default:
			err("Unrecognized option: %s", arg);
			break;
		}
		argc--;
		argv++;
	}

	gcos1();		/*setup initial GCOS cards.*/
	if(gcdebug)
		card('L', "$	select	cc/test/mail");
	else
		card('L', "$	select	cc/command/mail");
	if(argc == 1)
		errout("No recipients for mail");
	card('L', "mail - -s");
	while(--argc)
		card('L', *++argv);
	card('L', "$	data	i*,ncksum,copy");
	uname(&utsname);
	time(&tb);
	sprintf(cardbuf, "From %s on system %s     %s\n",
		&idcard[iname], utsname.nodename, ctime(&tb));
	if(copy(stdin, mailfile, MAIL) == -1)
		out();
	card('L', "$	endcopy");

	gcos2();		/*put out final cards, and spool job.*/
}

archive()
{
}


nuact()
{
}
