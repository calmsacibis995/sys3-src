#include <stdio.h>
#include <a.out.h>
#include "kas.h"

debug(in, x)
union {
	int	val;
	char *	ptr;
} x;
{
	register i, j, cnt;
	int	flag, fd;
	char	s[8];
	extern struct exec hdr;

	if (dbmode==0) {
		yyerror("not in debug mode");
		return;
	}
	switch(in) {
	case DLOAD:
		if (x.ptr == 0)
			x.ptr = "a.out";
		if ((fd = open(x.ptr, 0))<0) {
			printf("can not open %s\n", x.ptr);
			return;
		}
		for (i=0; i<NKMCI; i++) ispace[i] = 0;
		for (i=0; i<NKMCD; i++) dspace[i] = 0;
		read(fd, &hdr, sizeof hdr);
		if (hdr.a_magic==0440) {
			hdr.a_magic = 0410;
			lseek(fd, 64L, 0);
		}
		if (hdr.a_magic!=0410) {
			printf("bad format %s\n",x.ptr);
			close(fd);
			return;
		}
		read(fd, ispace, hdr.a_text);
		read(fd, dspace, hdr.a_data);
		lseek(dbfd, 0L, 0);
		write(dbfd, ispace, sizeof ispace);
		write(dbfd, dspace, sizeof dspace);
		close(fd);
		break;
	case DDUMP:
		if (x.ptr == 0)
			x.ptr = "core";
		if (getregs())
			return;
		if ((fd = creat(x.ptr, 0664))<0) {
			printf("can not create %s\n", x.ptr);
			return;
		}
		lseek(dbfd, 0L, 0);
		hdr.a_magic = 0440;
		hdr.a_text = read(dbfd, ispace, sizeof ispace);
		hdr.a_data = read(dbfd, dspace, sizeof dspace);
		hdr.a_syms = 0;
		write(fd, &hdr, sizeof hdr);
		write(fd, &ir, sizeof ir);
		write(fd, ispace, hdr.a_text);
		write(fd, dspace, hdr.a_data);
		close(fd);
		break;
	case DSTEP:
		cnt = x.val?x.val:1;
		s[1] = 0;
		for (i=1; i<=cnt; i++) {
			if (ksty(KSTEP, ir.csr, 0))
				return;
			flag = 0;
			for (j=0; j<8; j++)
				if (s[j]!=ir.csr[j]) {
					flag++;
					s[j] = ir.csr[j];
				}
			if (flag || i==cnt)
				printf("%4d%4o%4o%4o%4o%4o%4o%4o%4o\n",i,
				s[0]&0377,s[1]&0377,s[2]&0377,s[3]&0377,
				s[4]&0377,s[5]&0377,s[6]&0377,s[7]&0377);
		}
		break;
	case DRUN:
	case DSTOP:
	case DRESET:
		ksty(in, 0, x.val);
		break;
	case DREGS:
		getregs();
		printf("%4s:%4o%4o%4o%4o%4o%4o%4o%4o\n",
		"csr",ir.csr[0]&0377,ir.csr[1]&0377,ir.csr[2]&0377,ir.csr[3]&0377,
		ir.csr[4]&0377,ir.csr[5]&0377,ir.csr[6]&0377,ir.csr[7]&0377);
		printf("%4s:%4o%4o%4o%4o%4o%4o%4o%4o\n",
		"lur",ir.lur[0]&0377,ir.lur[1]&0377,ir.lur[2]&0377,ir.lur[3]&0377,
		ir.lur[4]&0377,ir.lur[5]&0377,ir.lur[6]&0377,ir.lur[7]&0377);
		printf("%4s:%4o%4o%4o%4o%4o%4o%4o%4o\n",
		"reg",ir.reg[0]&0377,ir.reg[1]&0377,ir.reg[2]&0377,ir.reg[3]&0377,
		ir.reg[4]&0377,ir.reg[5]&0377,ir.reg[6]&0377,ir.reg[7]&0377);
		printf("%4s:%4o%4o%4o%4o%4o%4o%4o%4o\n",
		"reg",ir.reg[8+0]&0377,ir.reg[8+1]&0377,ir.reg[8+2]&0377,ir.reg[8+3]&0377,
		ir.reg[8+4]&0377,ir.reg[8+5]&0377,ir.reg[8+6]&0377,ir.reg[8+7]&0377);
		printf("%4s:%4o%4o%4o%4o%4o%4o%4o%4o\n",
		"io",ir.io[0]&0377,ir.io[1]&0377,ir.io[2]&0377,ir.io[3]&0377,
		ir.io[4]&0377,ir.io[5]&0377,ir.io[6]&0377,ir.io[7]&0377);
		printf("%4s:%4o%4o%7s:%4o%7s:%4o\n",
		"npr",ir.npr[0]&0377, ir.npr[1]&0377,"brg",ir.brg&0377,"mem",ir.mem&0377);
		break;
	}
}

mstep(ins)
{
	char	csr[8];

	ksty(KMS, csr, ins);
	return(csr[2]);
}

getregs()
{
	register i;

	if (ksty(KCSR, ir.csr, 0))
		return(1);
	ir.brg = mstep(061222);
	ir.mem = mstep(041222);
	for (i=0; i<8; i++) {
		ir.lur[i] = mstep(021202|(i<<4));
		ir.io[i] = mstep(021002|(i<<4));
	}
	for (i=0; i<16; i++) {
		mstep(060600|i);
		ir.reg[i] = mstep(061222);
	}
	ir.npr[0] = mstep(0121202);
	ir.npr[1] = mstep(0121222);
	return(0);
}

ksty(tab)
{
	if (stty(dbfd, &tab)<0) {
		printf("%s not stopped\n",dbfile);
		return(1);
	}
	return(0);
}
