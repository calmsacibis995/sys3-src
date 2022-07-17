#include <stand.h>
#include <sys/filsys.h>

char	buf[5120];
char	nunix[14] = "unix";
char	ans[256];

char	*boot;
extern	rpboot ();

char	hp[] = "/dev/rp0";
char	ht[] = "/dev/mt0";

main ()
{
	register iocnt, count, rdev, wdev;
	register struct filsys *fs;
	register diskunit, tapeunit;

	printf ("\nUNIX -- Initial Load: Tape-to-Disk\n");
	printf("\nThe type of disk drive on which the Root file system will reside,\n");
	printf("as well as the type of tape drive that will be used for Tape 1\n");
	printf ("must be specified below.\n");
	printf ("\nAnswer the questions with a 'y' or 'n' followed by\n");
	printf ("a carriage return or line feed.\n");
	printf ("There is no type-ahead -- wait for the question to complete.\n");
	printf ("The character '@' will kill the entire line\n");
	printf ("while the character '#' will erase the last character typed.\n\n");
	if (yes ("RP06 at NEXUS 8")) {
		if ((wdev = open (hp, 1)) < 0) {
			perror (hp);
			exit (1);
		}
		boot = (char *) rpboot;
		strcat (nunix, "hp");
	} else {
		printf ("This release only supports the above disk drive.\n");
		printf ("Restart and answer correctly.\n\n");
		exit (1);
	}
	diskunit = number ("Drive number (0-7)");
	if (diskunit > 7) {
		printf ("Out of range; 0 assumed.\n");
		diskunit = 0;
	}
	_iobuf[wdev-3].i_dp->dt_unit = diskunit;
	printf ("Disk drive %d selected.\n", diskunit);
	printf ("\nMount a formatted pack on drive %d.\n", diskunit);
	while (!yes ("Ready"))
		printf ("So what's the matter?\n");
	putchar ('\n');
	if (!yes ("TE16 at NEXUS 9")) {
		printf ("This release only supports the above tape drives.\n");
		printf ("Restart and answer correctly.\n\n");
		exit (1);
	}
	if ((rdev = open (ht, 0)) < 0) {
		perror (ht);
		exit (1);
	}
	strcat (nunix, "ht");
	tapeunit = number ("Drive number (0-7)");
	if (tapeunit > 7) {
		printf ("Out of range; 0 assumed.\n");
		tapeunit = 0;
	}
	_iobuf[rdev-3].i_dp->dt_unit = tapeunit;
	printf ("Tape drive %d selected.\n", tapeunit);
	printf ("\nThe tape on drive %d will be read from the current position\n", tapeunit);
	printf ("at 800bpi, 5120 characters (10 blocks) per record,\n");
	printf ("and written onto the pack on drive %d starting at block 0.\n\n", diskunit);
	while (!yes ("Ready"))
		printf ("So what's the matter?\n");
	fs = (struct filsys *) &buf[512];
	fs->s_fsize = 0;
	if ((iocnt = read (rdev, buf, sizeof buf)) != sizeof buf) {
		printf ("Hard tape error -- abort\n", iocnt);
		printf ("read = %d in block %d\n", iocnt, (int) (tell (rdev) >> 9));
		exit (1);
	}
	if (fs->s_fsize)
		printf ("Size of filesystem to be copied is %d blocks.\n", fs->s_fsize);
	else {
		printf ("Bad format on tape -- abort\n");
		exit (1);
	}
	printf ("What is the pack volume label? (e.g. p0001): ");
	getline (ans);
	if (ans[0] == '\0')
		strcpy (ans, "p0001");
	ans[5] = '\0';
	printf ("The pack will be labeled %s.\n", ans);
	strcpy (fs->s_fpack, ans);
	count = (fs->s_fsize + 9) / 10;
	for (;;) {
		if ((iocnt = write (wdev, buf, sizeof buf)) != sizeof buf) {
			printf ("Hard disk error -- abort\n", iocnt);
			printf ("write = %d in block %d\n", iocnt, (int) (tell (wdev) >> 9));
			exit (1);
		}
		if (--count <= 0)
			break;
		if ((iocnt = read (rdev, buf, sizeof buf)) != sizeof buf) {
			printf ("Hard tape error -- abort\n", iocnt);
			printf ("read = %d in block %d\n", iocnt, (int) (tell (rdev) >> 9));
			exit (1);
		}
	}
	printf ("The boot block for your type of disk drive will now be installed.\n");
	lseek (wdev, (off_t) 0, 0);
	if ((iocnt = write (wdev, boot, 512)) != 512)
		printf ("Warning: cannot write boot block (write %d).\n", iocnt);
	printf ("\nThe file system copy is now complete.\n");
	printf ("\nTo boot the basic UNIX for your disk and tape drives\n");
	printf ("as indicated above, mount this pack on drive 0\n");
	printf ("and read in the boot block (block 0) using whatever\n");
	printf("means you have available; see vaxops(8).\n");
	printf ("\nThen boot the program %s using the stand-alone shell.\n", nunix);
	printf ("Normally:  $$ %s\n\n", nunix);
	printf ("The system will initially come up single-user; see init(8).\n");
	printf ("If you have an upper case only console terminal,\n");
	printf ("you must execute: stty lcase; see stty(1).\n");
	printf ("\nAfter UNIX is up, link the file %s to unix using ln(1).\n", nunix);
	printf ("        # ln /%s /unix\n", nunix);
	printf ("\nSet the date(1).\n");
	printf ("\nGood Luck!\n\n");
	printf ("The tape will now be rewound.\n\n");
	close (rdev);
	exit (0);
}

yes (qs)
char *qs; {

	printf (qs);
	printf ("?: ");
	getline (ans);
	if (ans[0] == 'y')
		return (1);
	if (ans[0] == 'n' || ans[0] == '\0')
		return (0);
	printf ("assumed 'n'\n");
	return (0);
}

number (qs)
char *qs; {
	register c, num;

	num = 0;
	printf (qs);
	printf ("?: ");
	getline (ans);
	c = ans[0];
	if ('0' <= c && c <= '9')
		num = num*10 + c - '0';
	return (num);
}

getline (s)
register char *s; {
	register char c;

	while ((c = getchar ()) != '\n')
		*s++ = c;
	*s = '\0';
}

exit (n) {

	if (n)
		_exit ();
	else
		asm ("	halt");
}
