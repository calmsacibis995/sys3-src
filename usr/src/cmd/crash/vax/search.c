#include	"crash.h"
#define	SYMTYPE	(sp->n_type & N_TYPE)

struct	nlist	*stbl;
unsigned  symcnt;
extern	char	*namelist;

struct	nlist	*
search(addr, m1, m2)
	long	addr;
	register  int  m1, m2;
{
	register  struct  nlist  *sp;
	register  struct  nlist  *save;
	unsigned	value;

/*	if((addr=atox(string)) == -1) {
		printf("invalid hex address: %s\n",string);
		return(NULL);
	}
*/
	value = 0;
	save = 0;
	for(sp = stbl; sp != &stbl[symcnt]; sp++) {
		if((SYMTYPE == m1 || SYMTYPE == m2) &&
			SYM_VALUE(sp) <= (addr & ~HIGHBIT) &&
			SYM_VALUE(sp) > value) {
			value = SYM_VALUE(sp);
			save = sp;
		}
	}
	return(save);
}

struct	nlist	*
symsrch(s)
	register  char  *s;
{
	register  struct  nlist  *sp;
	register  struct  nlist  *found;

	found = NULL;
	for(sp = stbl; sp != &stbl[symcnt]; sp++) {
		if(strncmp(sp->n_name,s,8) == 0) {
			found = sp;
			break;
		}
	}
	return(found);
}

struct	nlist	*
nmsrch(s)
	register  char  *s;
{
	char	ct[20];
	register  struct  nlist  *sp;

	if(strlen(s) > 19)
		return(0);
	if((sp = symsrch(s)) == NULL) {
		strcpy(ct, "_");
		strcat(ct, s);
		sp = symsrch(ct);
	}
	return(sp);
}

prnm(s)
	register  char  *s;
{
	register  char  *cp;
	register  struct  nlist  *sp;

	printf("%-10.10s ", s);
	if(strlen(s) > 8) {
		printf("bad name\n");
		return;
	}
	if((sp = nmsrch(s)) == NULL) {
		printf("no match\n");
		return;
	}
	printf("%08.8lx  ", sp->n_value);
	switch(SYMTYPE) {
	case N_UNDF | N_EXT:
	case N_UNDF:
		cp = "undefined"; break;
	case N_ABS | N_EXT:
	case N_ABS:
		cp = "absolute"; break;
	case N_TEXT | N_EXT:
	case N_TEXT:
		cp = "text"; break;
	case N_DATA | N_EXT:
	case N_DATA:
		cp = "data"; break;
	case N_BSS | N_EXT:
	case N_BSS:
		cp = "bss"; break;
	case N_FN:
		cp = "ld file name"; break;
	default:  cp = "unknown?"; break;
	}
	printf("%s\n", cp);
}

prod(addr, units, style)
	long	addr;
	int	units;
	char	*style;
{
	register  int  i;
	register  struct  prmode  *pp;
	int	word;
	long	lword;
	char	ch;
	extern	struct	prmode	prm[];

	if(units == -1)
		return;
	for(pp = prm; pp->pr_sw != 0; pp++)
		if(strcmp(pp->pr_name, style) == 0)
			break;
	if(lseek(kmem, (long)(addr & ~HIGHBIT), 0) == -1) {
		error("bad seek of addr");
	}
	switch(pp->pr_sw) {
	default:
	case NULL:
		error("invalid mode");
		break;

	case OCTAL:
	case DECIMAL:
		if(addr & 01) {
			printf("warning: word alignment performed\n");
			addr &= ~01;
			if(lseek(kmem, (long)(addr & ~HIGHBIT), 0) == -1) {
				error("bad seek of addr");
			}
		}
		for(i = 0; i < units; i++) {
			if(i % 8 == 0) {
				if(i != 0)
					putc('\n', stdout);
				printf("%7.7lx:", addr + i * NBPW);
			}
			if(read(kmem, &word, NBPW) != NBPW) {
				printf("  read error");
				break;
			}
			printf(pp->pr_sw == OCTAL ? " %7.7o" :
				"  %5u", word & ~HIGHBIT);
		}
		break;

	case LOCT:
	case LDEC:
		if(addr & 01) {
			printf("warning: word alignment performed\n");
			addr &= ~01;
			if(lseek(kmem, (long)(addr & ~HIGHBIT), 0) == -1) {
				error("bad seek of addr");
			}
		}
		for(i = 0; i < units; i++) {
			if(i % 4 == 0) {
				if(i != 0)
					putc('\n', stdout);
				printf("%7.7lx:", addr + i * NBPW);
			}
			if(read(kmem, &lword, sizeof (long)) != sizeof (long)) {
				printf("  read error");
				break;
			}
			printf(pp->pr_sw == LOCT ? " %12.12lo" :
				"  %10lu", lword);
		}
		break;

	case CHAR:
	case BYTE:
		for(i = 0; i < units; i++) {
			if(i % (pp->pr_sw == CHAR ? 16 : 8) == 0) {
				if(i != 0)
					putc('\n', stdout);
				printf("%7.7lx: ", addr + i * sizeof (char));
			}
			if(read(kmem, &ch, sizeof (char)) != sizeof (char)) {
				printf("  read error");
				break;
			}
			if(pp->pr_sw == CHAR)
				putch(ch);
			else
				printf(" %4.4o", ch & 0377);
		}
		break;
	case HEX:
		if(addr & 01) {
			printf("warning: word alignment performed\n");
			addr &= ~01;
			if(lseek(kmem, (long)(addr & ~HIGHBIT), 0) == -1) {
				error("bad seek of addr");
			}
		}
		for(i = 0; i < units; i++) {
			if(i % 4 == 0) {
				if(i != 0)
					putc('\n', stdout);
				printf("%7.7lx:", addr + i * NBPW);
			}
			if(read(kmem, &lword, sizeof (long)) != sizeof (long)) {
				printf("  read error");
				break;
			}
			printf(" %08lx", lword);
		}
		break;


	}
	putc('\n', stdout);
}
