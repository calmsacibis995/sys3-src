/*
 *	prfld - load profiler with sorted kernel text addresses
 */

# include "a.out.h"
# include "errno.h"

# define PRFMAX 512

char	*namelist = "/unix";
struct	nlist	symbol;
struct	exec	aout;
unsigned	symcnt;
int	symloc;
int	taddr[PRFMAX];
extern	int	errno;

main(argc, argv)
	char	**argv;
{
	register  int  *ip, nm, prf;
	int	compar();

	if(argc == 2)
		namelist = argv[1];
	else if(argc != 1)
		error("usage: prfld [/unix]");
	if((nm = open(namelist, 0)) < 0)
		error("cannot open namelist file");
	if((prf = open("/dev/prf", 1)) < 0)
		error("cannot open /dev/prf");
	if(read(nm, &aout, sizeof aout) != sizeof aout)
		error("cannot read namelist file");
	if(aout.a_syms == 0)
		error("no namelist");

	symcnt = aout.a_syms / sizeof symbol;
	symloc = 32 + aout.a_text + aout.a_data + aout.a_trsize + aout.a_drsize;
	lseek(nm, symloc, 0);
	ip = taddr;
	*ip++ = 0;
	while(symcnt--) {
		if(ip == &taddr[PRFMAX])
			error("too many text symbols");
		if(read(nm, &symbol, sizeof symbol) != sizeof symbol)
			error("cannot read namelist");
		if(symbol.n_type == (N_TEXT | N_EXT))
			*ip++ = symbol.n_value;
	}
	qsort(taddr, ip - taddr, sizeof (int), compar);
	if(write(prf, taddr, (unsigned)ip - (unsigned)taddr) !=
	    (unsigned)ip - (unsigned)taddr)
		switch(errno) {
		case ENOSPC:
			error("insufficient space in system for addresses");
		case E2BIG:
			error("unaligned data or insufficient addresses");
		case EBUSY:
			error("profiler is enabled");
		case EINVAL:
			error("text addresses not sorted properly");
		default:
			error("cannot load profiler addresses");
		}
}

compar(x, y)
	register  unsigned  *x, *y;
{
	if(*x > *y)
		return(1);
	else if(*x == *y)
		return(0);
	return(-1);
}

error(s)
	register  char  *s;
{
	write(2, "prfld: ", 7);
	write(2, s, strlen(s));
	write(2, "\n", 1);
	exit(1);
}
