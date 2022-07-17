#include	"crash.h"
#include	"sys/inode.h"
#include	"sys/proc.h"
#include	"sys/file.h"
#include	"sys/mount.h"
#include	"sys/text.h"

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
	extern	struct	glop	dumglop;
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
	if(abuf.a_flag)
		symloc = 020L + (long)abuf.a_text + (long)abuf.a_data;
	else
		symloc = 020L + 2L * ((long)abuf.a_text + (long)abuf.a_data);
	symcnt = abuf.a_syms / sizeof (struct nlist);
	if((stbl = (struct nlist *)sbrk(abuf.a_syms)) == -1)
		fatal("cannot allocate space for namelist");
	lseek(nmfd, symloc, 0);
	if(read(nmfd, stbl, abuf.a_syms) != abuf.a_syms)
		fatal("cannot read namelist");
	close(nmfd);
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
	if(ka6 == -1) {
		if(strcmp(dumpfile, "/dev/mem") == 0)
			ka6 = (((long)Etext->n_value - 8192 + 63) & ~077) +
				(((long)End->n_value + 63) & ~077);
		else
			ka6 = (long) dumpglop.g_ka6 << 6;
	}
	lseek(kmem, (long)V->n_value, 0);
	if(read(kmem, &v, sizeof v) != sizeof v)
		error("read error on v structure");
	v.ve_inode = ((unsigned)v.ve_inode - Inode->n_value) /
		sizeof (struct inode);
	v.ve_file = ((unsigned)v.ve_file - File->n_value) /
		sizeof (struct file);
	v.ve_mount = ((unsigned)v.ve_mount - Mount->n_value) /
		sizeof (struct mount);
	v.ve_proc = ((unsigned)v.ve_proc - Proc->n_value) /
		sizeof (struct proc);
	v.ve_text = ((unsigned)v.ve_text - Text->n_value) /
		sizeof (struct text);
	if(symsrch("_kl_tty"))
		settty("kl");
	signal(SIGINT, sigint);
}
