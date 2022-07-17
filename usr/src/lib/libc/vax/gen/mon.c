/*LINTLIBRARY*/
monitor(lowpc, highpc, buffer, bufsiz, nfunc)
register char *lowpc, *highpc;
int buffer[];
register int bufsiz;
{
	register int oh;
	static int *sbuf, ssiz;
	struct phdr {
		int *lpc;
		int *hpc;
		int ncnt;
	};
	struct cnt {
		int *pc;
		long ncall;
	};

	if(lowpc == 0) {
		profil((char *) 0, 0, 0, 0);
		oh = creat("mon.out", 0666);
		write(oh, (char *) sbuf, ssiz);
		close(oh);
		return;
	}
	sbuf = buffer;
	ssiz = bufsiz;
	buffer[0] = (int) lowpc;
	buffer[1] = (int) highpc;
	buffer[2] = nfunc;
	oh = sizeof(struct phdr) + nfunc * sizeof(struct cnt);
	buffer = (int *) (((int) buffer) + oh);
	bufsiz -= oh;
	if(bufsiz <= 0)
		return;
#ifdef	vax
	if(bufsiz > 0xffff)
		bufsiz = 0xffff;
#endif
	oh = (((unsigned) (highpc+1-lowpc))>>1 & 0x7fff);
	if(bufsiz < oh)
		oh = ((long) bufsiz<<15) / oh;
	else
		oh = 0xffff;
	profil((char *) buffer, bufsiz, (int) lowpc, oh);
}
