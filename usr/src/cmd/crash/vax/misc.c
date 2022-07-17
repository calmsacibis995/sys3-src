#include	"crash.h"
#include	"sys/inode.h"
#include	"sys/proc.h"
#include	"sys/file.h"
#include	"sys/mount.h"
#include	"sys/text.h"
#if vax
#define	SEGBYTES	0x40000000L	/* power of 2 to the 30th */
#define	CORECLKS	512		/* Core Clicks */
#define	BITMASK		0777		/* Remove low order bits */
#endif
#if pdp11
#define	SEGBYTES	8192
#define CORECLKS	64
#define	BITMASK		077
#endif

extern	struct	glop	dumpglop;

strcpy8(to, from)
	register  char  *to, *from;
{
	register  int  i;

	for(i=0; i<8; i++) {
		if((*to++ = *from++) == '\0') {
			while(++i < 8)
				*to++ = '\0';
		}
	}
}

fatal(str)
	char	*str;
{
	printf("error: %s\n", str);
	exit(1);
}

error(str)
	char	*str;
{
	printf("error: %s\n", str);
}

atoi(s)
	register  char  *s;
{
	register  base, numb;

	numb = 0;
	if(*s == '0')
		base = 8;
	else
		base = 10;
	while(*s) {
		if(*s < '0' || *s > '9') {
			error("number expected");
			return(-1);
		}
		numb = numb * base + *s++ - '0';
	}
	return(numb);
}

long
atol(s)
	register  char  *s;
{
	long	numb;

	if(*s < '0' || *s > '9') {
		error("invalid ka6");
		return(ka6 >> 6);
	}
	numb = 0;
	while(*s >= '0' && *s <= '9')
		numb = (numb << 3) + *s++ - '0';
	return(numb);
}

init()
{
	extern	char	*dumpfile;
	extern	struct	glop	dumpglop;
	extern	int	r5;
	extern	char	*namelist;
	extern	struct	nlist	*stbl;
	extern  unsigned  symcnt;
	extern	struct	var	v;

	struct	exec	abuf;
	int	nmfd;
	long	symloc;
	char	*sbrk();
	int	sigint();
	struct	nlist	*symsrch();

	if((kmem = open(dumpfile, 0)) < 0)
		fatal("cannot open dump file");
	if(read(kmem, &dumpglop, sizeof dumpglop) != sizeof dumpglop)
		error("cannot read dump registers");
	r5 = dumpglop.g_r5;
	nmfd = open(namelist, 0);
	if(read(nmfd, &abuf, sizeof abuf) != sizeof abuf)
		fatal("cannot read namelist file");

	if(BADMAG(abuf))
		fatal("namelist not in a.out format");

	symloc = (long) abuf.a_text + abuf.a_data;
#ifdef	vax
	symloc += (long) abuf.a_trsize + abuf.a_drsize;
#else
	if ((abuf.a_flag & 01) == 0)
		symloc *= 2;
#endif
	symcnt = abuf.a_syms / sizeof (struct nlist);
	if((stbl = (struct nlist *)sbrk(abuf.a_syms)) == (struct nlist *)-1)
		fatal("cannot allocate space for namelist");
	lseek(nmfd, symloc, 1);
	if(read(nmfd, stbl, abuf.a_syms) != abuf.a_syms)
		fatal("cannot read namelist");
	close(nmfd);
	Dmpstk = symsrch("dumpstack");
	Umap = symsrch("_Umap");
	U = symsrch("_u");
	File = symsrch("_file");
	Inode = symsrch("_inode");
	Mount = symsrch("_mount");
	Proc = symsrch ("_proc");
	Text = symsrch("_text");
	Swap = symsrch("_swapmap");
	Buf = symsrch("_buf");
	Core = symsrch("_coremap");
	V = symsrch("_v");
	Sys = symsrch("_utsname");
	Time = symsrch("_time");
	Lbolt = symsrch("_lbolt");
	Panic = symsrch("_panicstr");
	Etext = symsrch("_etext");
	End = symsrch("_end");
	Callout = symsrch("_callout");
	lseek(kmem, (long)SYM_VALUE(V), 0);
	if(read(kmem, &v, sizeof v) != sizeof v)
		error("read error on v structure");
	v.ve_inode = (char *)(((unsigned)v.ve_inode - Inode->n_value) /
		sizeof (struct inode));
	v.ve_file = (char *)(((unsigned)v.ve_file - SYM_VALUE(File)) /
		sizeof (struct file));
	v.ve_mount = (char *)(((unsigned)v.ve_mount - SYM_VALUE(Mount)) /
		sizeof (struct mount));
	v.ve_proc = (char *)(((unsigned)v.ve_proc - SYM_VALUE(Proc)) /
		sizeof (struct proc));
	v.ve_text = (char *)(((unsigned)v.ve_text - SYM_VALUE(Text)) /
		sizeof (struct text));
#ifdef	vax
	if(symsrch("_con_tty"))
		settty("dz");
#endif
#ifdef	pdp11
	if(symsrch("_kl_tty"))
		settty("kl");
#endif
	signal(SIGINT, sigint);
}

getuarea(slot)
long	slot;
{
	struct	proc pbuf;
	int	i;
	int	fd;
	union u_pag {
		char	cpg_addr[4];
		long	pg_addr;
	} upag ;

	if(slot == -1) {	/* Gather last executing process */
		for(i=0; i < UPAGES; i++) {
			lseek(kmem,((long)SYM_VALUE(Umap)+i*sizeof(long)),0);
			read(kmem,upag.cpg_addr,sizeof(union u_pag));
			lseek(kmem,(long)upag.pg_addr<<9,0);
			if(read(kmem,&x.stk[512*i],512) != 512) {
				return(-1);
			}
		}
		return(0);
	}

	lseek(kmem,(long)SYM_VALUE(Proc)+slot*sizeof(struct proc),0);
	if(read(kmem, &pbuf, sizeof(pbuf)) != sizeof(pbuf)) {
		printf("read error on proc table\n");
		return(-1);
	}

	for(i=0; i < UPAGES; i++) {
		lseek(kmem,(pbuf.p_addr[i]<<9),0);
		if(read(kmem,&x.stk[512*i],512) != 512) {
			return(-1);
		}
	}
	return(0);
}

long
atox(string)
char	*string;
{
	register int	i;
	register int	j;
	register int	k;
	long	value, prod, pwr;

	for(i=strlen(string)-1,j=0; i>=0; i--,j++) {
		switch (string[j]) {
			case '0':	value =  0; break;
			case '1':	value =  1; break;
			case '2':	value =  2; break;
			case '3':	value =  3; break;
			case '4':	value =  4; break;
			case '5':	value =  5; break;
			case '6':	value =  6; break;
			case '7':	value =  7; break;
			case '8':	value =  8; break;
			case '9':	value =  9; break;
			case 'a':
			case 'A':	value = 10; break;
			case 'b':
			case 'B':	value = 11; break;
			case 'c':
			case 'C':	value = 12; break;
			case 'd':
			case 'D':	value = 13; break;
			case 'e':
			case 'E':	value = 14; break;
			case 'f':
			case 'F':	value = 15; break;
			default:	return(-1);
		}

		if(i == 0) return(prod += value);
		else {
			for(k=i,pwr=16; k>1; k--) pwr *= pwr;
			prod += pwr*value;
		}
	}
}
