#include	"crash.h"
#include	"sys/inode.h"
#include	"sys/proc.h"
#include	"sys/file.h"
#include	"sys/mount.h"
#include	"sys/text.h"
#include	"sys/utsname.h"

struct	glop	dumpglop;
struct	dstk	dmpstack;

prstat()
{
	long	toc, lbolt;
	unsigned  panicstr;
	struct	utsname	utsname;
	char	panic[30];

	lseek(kmem, (long)SYM_VALUE(Sys), 0);
	if(read(kmem, &utsname, sizeof utsname) != sizeof utsname)
		printf("read error on uname structure\n");
	else {
		printf("\tsystem name: %.8s\n", utsname.sysname);
		printf("\tnode name: %.8s\n", utsname.nodename);
		printf("\trelease: %.8s\n", utsname.release);
		printf("\tversion: %.8s\n", utsname.version);
	}
	lseek(kmem, (long)SYM_VALUE(Time), 0);
	if(read(kmem, &toc, sizeof toc) != sizeof toc)
		toc = 0;
	printf("\ttime of crash: %s", ctime(&toc));
	lseek(kmem, (long)SYM_VALUE(Lbolt), 0);
	if(read(kmem, &lbolt, sizeof lbolt) != sizeof lbolt)
		lbolt = 0;
	printf("\tage of system: ");
	lbolt = (lbolt + (long) 3599) / (long) 3600;
	if(lbolt / (long)(60 * 24))
		printf("%lu day, ", lbolt / (long)(60 * 24));
	lbolt %= (long)(60 * 24);
	if(lbolt / (long)60)
		printf("%lu hr., ", lbolt / (long)60);
	lbolt %= (long) 60;
	if(lbolt)
		printf("%lu min.", lbolt);
	printf("\n");
	lseek(kmem, (long)SYM_VALUE(Panic), 0);
	if(read(kmem, &panicstr, sizeof panicstr) == sizeof panicstr &&
		panicstr != 0) {
		lseek(kmem, (long)panicstr, 0);
		if(read(kmem, panic, 30) == 30)
		printf("\tpanic: %.30s\n\n", panic);
	}
#ifdef	pdp11
	printf("\tlow memory:\n");
	printf("\t00: %08x  02: %08x  r0: %08x  r1: %08x  r2:  %08x\n",
		dumpglop.g_x0, dumpglop.g_x1, dumpglop.g_r0,
		dumpglop.g_r1, dumpglop.g_r2);
	printf("\tr3: %08x  r4: %08x  r5: %08x  sp: %08x  ka6: %08x\n",
		dumpglop.g_r3, dumpglop.g_r4, dumpglop.g_r5,
		dumpglop.g_sp, dumpglop.g_ka6);
#endif

#ifdef	vax
	lseek(kmem,(long)(SYM_VALUE(Dmpstk) - sizeof(struct dstk)),0);
	if(read(kmem,&dmpstack,sizeof(dmpstack)) != sizeof(dmpstack)) {
		error("read error on dumpstack");
		return(-1);
	}

	printf("\nDUMPSTACK:\tstkptr: %lx  pcbb: %lx  mapen: %lx  ipl: %lx",
		dmpstack.stkptr,dmpstack.pcbb,dmpstack.mapen,dmpstack.ipl);
	printf("\n    r0:  %08lx  r1:  %08lx  r2:  %08lx  r3:  %08lx  r4:  %08lx",
		dmpstack.r0, dmpstack.r1, dmpstack.r2, dmpstack.r3, dmpstack.r4);
	printf("\n    r5:  %08lx  r6:  %08lx  r7:  %08lx  r8:  %08lx  r9:  %08lx",
		dmpstack.r5,dmpstack.r6,dmpstack.r7,dmpstack.r8,dmpstack.r9);
	printf("\n    r10: %08lx  r11: %08lx  r12: %08lx  r13: %08lx\n",
		dmpstack.r10,dmpstack.r11,dmpstack.r12,dmpstack.r13);
#endif
}

prvar()
{
	extern	struct	var	v;

	lseek(kmem, (long)SYM_VALUE(V), 0);
	if(read(kmem, &v, sizeof v) != sizeof v) {
		error("read error on v structure");
		return;
	}
	v.ve_inode = ((unsigned)v.ve_inode - Inode->n_value) /
		(sizeof (struct inode));
	v.ve_file = ((unsigned)v.ve_file - File->n_value) /
		sizeof (struct file);
	v.ve_mount = ((unsigned)v.ve_mount - Mount->n_value) /
		sizeof (struct mount);
	v.ve_proc = ((unsigned)v.ve_proc - Proc->n_value) /
		sizeof (struct proc);
	v.ve_text = ((unsigned)v.ve_text - Text->n_value) /
		sizeof (struct text);
	printf("buffers: %3d, sabufs: %3d,   calls: %3d, clists: %3d,",
		v.v_buf, v.v_sabuf, v.v_call, v.v_clist);
	printf("  maxproc: %3d,\n", v.v_maxup);
	printf(" inodes: %3d,  files: %3d,  mounts: %3d,  ",
		v.v_inode, v.v_file, v.v_mount);
	printf("procs: %3d,    texts: %3d,\n", v.v_proc, v.v_text);
	printf("e_inode: %3d, e_file: %3d, e_mount: %3d,",
		v.ve_inode, v.ve_file, v.ve_mount);
	printf(" e_proc: %3d,   e_text: %3d;\n", v.ve_proc, v.ve_text);
}
