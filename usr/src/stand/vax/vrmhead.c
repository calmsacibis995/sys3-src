struct wd {
	short hiword;
	short loword;
};

fixl (p)
register long *p; 
{
	register short t;

	t = ((struct wd *) p)->hiword;
	((struct wd *) p)->hiword = ((struct wd *) p)->loword;
	((struct wd *) p)->loword = t;
}

main (argc, argv)
char *argv[]; 
{
	int ifd, ofd, nbyt, ttt;
	register char *infile, *oufile, *ap;
	char *fbuf, zero;
	struct {
		long magic;
		long tlen, dlen;
		long blen, slen, eadr, trl, drl;
	} 
	head;
	char buf[512];
	char *sbrk ();

	zero = '\0';
	if (argc != 3) {
usage:
		printf ("usage: vrmhead input output\n");
		exit (1);
	}

	infile = argv[1];
	oufile = argv[2];

	if ((ifd = open (infile, 0)) < 0) {
		ap = infile;
oerr:
		printf ("%s: cannot open\n", ap);
		exit (1);
	}

	if ((ofd = creat (oufile, 0777)) < 0) {
		ap = oufile;
		goto oerr;
	}

	read (ifd, &head, sizeof head);
#ifndef vax
	fixl (&head.tlen); 
	fixl (&head.dlen);
#endif
	if (lseek (ifd, sizeof head, 0) < 0) {
		printf ("input seek error\n");
unlnk:
		unlink (oufile);
		exit (1);
	}

	fbuf = sbrk (4096);
	while (head.tlen > 0) {
		if (head.tlen > 4096)
			nbyt = 4096; 
		else
			nbyt = head.tlen;
		read (ifd, fbuf, nbyt);
		write (ofd, fbuf, nbyt);
		head.tlen -= nbyt;
	}
	while (nbyt & 0777) {
		write (ofd, &zero, 1); 
		++nbyt;
	}
	while (head.dlen > 0) {
		if (head.dlen > 4096)
			nbyt = 4096; 
		else
			nbyt = head.dlen;
		read (ifd, fbuf, nbyt);
		write (ofd, fbuf, nbyt);
		head.dlen -= nbyt;
	}
	return (0);
}
