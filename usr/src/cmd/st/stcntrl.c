#define NUMDEV	1

struct st {
	char s_spa;
	char s_ssa;
	char s_typet;
	char s_dev;
	unsigned s_mode;
} st;

struct kmcline {
	int sdev;
	char init[9];
} kmcline[NUMDEV] =
{0, "\126 -\003"};

char schan[9] = "/dev/stX";

main(argc,argv)
int argc;
char *argv[];
{
	register int fd, i;

	if (argc != 2)
		usage();
	for(i=0; i<NUMDEV; i++) {
		schan[7] = i + '0';
		st.s_dev = kmcline[i].sdev;
		st.s_spa = st.s_ssa = st.s_typet = 0;
		if ((fd = open(schan,2)) < 0) {
			printf("stcntrl: cannot open %s\n",schan);
			exit(1);
		}
		if (strcmp(argv[1],"off") == 0) {
			st.s_mode = 1;
			if (ioctl(fd,0,&st)) {
				printf ("stcntrl: ioctl failed\n");
				exit(1);
			}
			continue;
		}
		else if (strcmp(argv[1],"on") == 0) {
			st.s_mode = 0;
			if (ioctl(fd,0,&st)) {
				printf ("stcntrl: ioctl failed\n");
				exit(1);
			}
			write(fd,&kmcline[i].init[0],strlen(&kmcline[i].init[0]));
			close(fd);
		}
		else
			usage();
	}
}

usage() {
	printf ("Usage: stcntrl on | off\n");
	exit(1);
}
