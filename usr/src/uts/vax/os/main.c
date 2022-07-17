#include "sys/param.h"
#include "sys/systm.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/filsys.h"
#include "sys/mount.h"
#include "sys/proc.h"
#include "sys/inode.h"
#include "sys/seg.h"
#include "sys/conf.h"
#include "sys/buf.h"
#include "sys/iobuf.h"
#include "sys/tty.h"
#include "sys/var.h"
#include "sys/mtpr.h"
#include "sys/page.h"
#include "sys/clock.h"

/*
 * Initialization code.
 * Called from cold start routine as
 * soon as a stack and segmentation
 * have been established.
 * Functions:
 *	clear and free user core
 *	turn on clock
 *	hand craft 0th process
 *	call all initialization routines
 *	fork - process 0 to schedule
 *	     - process 1 execute bootstrap
 *
 * loop at low address in user mode -- /etc/init
 *	cannot be executed.
 */
main(firstaddr)
{
	int i;

	startup(firstaddr);

	/*
	 * set up system process
	 */

	for(i=0; i<UPAGES; i++) {
		proc[0].p_addr[i] = firstaddr + i;
	}
	proc[0].p_size = UPAGES;
	proc[0].p_stat = SRUN;
	proc[0].p_flag |= SLOAD|SSYS;
	proc[0].p_nice = NZERO;
	u.u_procp = &proc[0];
	u.u_cmask = CMASK;
	u.u_limit = CDLIMIT;

	/*
	 * initialize system tables
	 */

	clkstart();
	cinit();
	binit();
	errinit();
	iinit();
	rootdir = iget(rootdev, ROOTINO);
	rootdir->i_flag &= ~ILOCK;
	u.u_cdir = iget(rootdev, ROOTINO);
	u.u_cdir->i_flag &= ~ILOCK;
	u.u_rdir = NULL;
	u.u_start = time;

	/*
	 * make init process
	 * enter scheduling loop
	 * with system process
	 */

	if(newproc()) {
		expand(btoc(szicode), P0BR);
		u.u_dsize = btoc(szicode);
		copyout((caddr_t)icode, (caddr_t)0, szicode);
		/*
		 * Return goes to loc. 0 of user init
		 * code just copied out.
		 */
		return;
	}
	sched();
}

/*
 * iinit is called once (from main) very early in initialization.
 * It reads the root's super block and initializes the current date
 * from the last modified date.
 *
 * panic: iinit -- cannot read the super block.
 * Usually because of an IO error.
 */
iinit()
{
	register struct buf *cp, *bp;
	register struct filsys *fp;

	(*bdevsw[major(rootdev)].d_open)(minor(rootdev), 1);
	(*bdevsw[major(pipedev)].d_open)(minor(pipedev), 1);
	(*bdevsw[major(swapdev)].d_open)(minor(swapdev), 1);
	bp = bread(rootdev, SUPERB);
	cp = geteblk();
	if(u.u_error)
		panic("iinit");
	bcopy(bp->b_un.b_addr, cp->b_un.b_addr, sizeof(struct filsys));
	brelse(bp);
	mount[0].m_bufp = cp;
	mount[0].m_flags = MINUSE;
	mount[0].m_dev = rootdev;
	fp = cp->b_un.b_filsys;
	fp->s_flock = 0;
	fp->s_ilock = 0;
	fp->s_ronly = 0;
	fp->s_ninode = 0;
	fp->s_inode[0] = 0;

	clkset(fp->s_time);
}

/*
 * Initialize clist by freeing all character blocks.
 */
struct chead cfreelist;
cinit()
{
	register n;
	register struct cblock *cp;

	for(n = 0, cp = &cfree[0]; n < v.v_clist; n++, cp++) {
		cp->c_next = cfreelist.c_next;
		cfreelist.c_next = cp;
	}
	cfreelist.c_size = CLSIZE;
}

/*
 * Initialize the buffer I/O system by freeing
 * all buffers and setting all device buffer lists to empty.
 */
binit()
{
	register struct buf *bp;
	register struct buf *dp;
	register unsigned i;
	struct bdevsw *bdp;

	dp = &bfreelist;
	dp->b_forw = dp->b_back =
	    dp->av_forw = dp->av_back = dp;
	for (i=0, bp=buf; i<v.v_buf; i++,bp++) {
		bp->b_dev = NODEV;
		bp->b_un.b_addr = buffers[i];
		bp->b_back = dp;
		bp->b_forw = dp->b_forw;
		dp->b_forw->b_back = bp;
		dp->b_forw = bp;
		bp->b_flags = B_BUSY;
		bp->b_bcount = BSIZE;
		brelse(bp);
	}
	for (i=0, bdp = bdevsw; i<bdevcnt; i++, bdp++) {
		dp = (struct buf *)bdp->d_tab;
		if (dp) {
			dp->b_forw = dp;
			dp->b_back = dp;
		}
	}
	for (i=0; i < v.v_hbuf; i++ )
		hbuf[i].b_forw = hbuf[i].b_back = (struct buf *)&hbuf[i];
}
