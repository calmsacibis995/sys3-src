/*
 *	prfpr - print profiler log files
 */

# include "time.h"
# include "a.out.h"

# define PRFMAX  512		/* max number of text symbols */

struct	profile	{
	long	p_date;		/* time stamp of record */
	long	p_ctr[PRFMAX];	/* profiler counter values */
} p[2];

struct	nlist	*stbl, *sp, *search();

unsigned  symcnt;
int	prfmax;			/* actual number of text symbols */

int	t[PRFMAX];

char	*namelist = "/unix";
char	*logfile;

long	sum, osum;

double	pc;
double	cutoff = 1e-2;

main(argc, argv)
	char	**argv;
{
	register  int  ff, log, i;
	double	atof();

	switch(argc) {
		case 4:
			namelist = argv[3];
		case 3:
			cutoff = atof(argv[2]) / 1e2;
		case 2:
			logfile = argv[1];
			break;
		default:
			error("usage: prfpr file [ cutoff [ namelist ] ]");
	}
	if((log = open(logfile, 0)) < 0)
		error("cannot open data file");
	if(cutoff >= 1e0 || cutoff < 0e0)
		error("invalid cutoff percentage");
	if(read(log, &prfmax, sizeof prfmax) != sizeof prfmax || prfmax == 0)
		error("bad data file");
	if(read(log, t, prfmax * sizeof (int)) != prfmax * sizeof (int))
		error("cannot read profile addresses");
	osum = sum = ff = 0;
	init();

	read(log, &p[!ff], (prfmax + 2) * sizeof (int));
	for(i = 0; i <= prfmax; i++)
		osum += p[!ff].p_ctr[i];

	for(;;) {
		sum = 0;
		if(read(log, &p[ff], (prfmax + 2) * sizeof (int)) !=
		    (prfmax + 2) * sizeof (int))
			exit(0);
		shtime(&p[!ff].p_date);
		shtime(&p[ff].p_date);
		printf("\n");
		for(i = 0; i <= prfmax; i++)
			sum += p[ff].p_ctr[i];
		if(sum == osum)
			printf("no samples\n\n");
		else for(i = 0; i <= prfmax; i++) {
			pc = (double) (p[ff].p_ctr[i] - p[!ff].p_ctr[i]) /
				(double) (sum - osum);
			if(pc > cutoff)
				if(i == prfmax)
					printf("user     %5.2f\n",
						1e2 * pc);
				else {
					sp = search(t[i], N_TEXT|N_EXT,
					    N_TEXT|N_EXT);
					if(sp == 0)
						printf("unknown  %5.2f\n",
							pc * 1e2);
					else if(sp->n_name[0] == '_')
						printf("%-7.7s  %5.2f\n",
							&sp->n_name[1],
							pc * 1e2);
					else
						printf("%-8.8s %5.2f\n",
							sp->n_name,
							pc * 1e2);
				}
		}
		ff = !ff;
		osum = sum;
		printf("\n");
	}
}

error(s)
	char	*s;
{
	printf("error: %s\n", s);
	exit(1);
}

struct	nlist	*
search(addr, m1, m2)
	unsigned  addr;
	register  int  m1, m2;
{
	register  struct  nlist  *sp;
	register  struct  nlist  *save;
	unsigned	value;

	value = 0;
	save = 0;
	for(sp = stbl; sp != &stbl[symcnt]; sp++)
		if((sp->n_type == m1 || sp->n_type == m2) &&
			sp->n_value <= addr && sp->n_value > value) {
			value = sp->n_value;
			save = sp;
		}
	return(save);
}

init()
{
	register  int  nmfd;
	long	symloc;
	struct	exec	abuf;

	if((nmfd = open(namelist, 0)) < 0)
		error("cannot open namelist");
	if(read(nmfd, &abuf, sizeof abuf) != sizeof abuf)
		error("cannot read exec header");
	symloc = 32+abuf.a_text+abuf.a_data+abuf.a_trsize+abuf.a_drsize;
	symcnt = abuf.a_syms / sizeof (struct nlist);
	if((stbl = (struct nlist *) sbrk(abuf.a_syms)) ==
	    (struct  nlist  *) -1)
		error("cannot allocate space for namelist");
	lseek(nmfd, symloc, 0);
	if(read(nmfd, stbl, abuf.a_syms) != abuf.a_syms)
		error("cannot read symbol table");
	close(nmfd);
}

shtime(l)
	register  long  *l;
{
	register  struct  tm  *t;
	struct  tm  *localtime();

	if(*l == (long) 0) {
		printf("initialization\n");
		return;
	}
	t = localtime(l);
	printf("%02.2d/%02.2d/%02.2d %02.2d:%02.2d\n", t->tm_mon + 1,
		t->tm_mday, t->tm_year, t->tm_hour, t->tm_min);
}
