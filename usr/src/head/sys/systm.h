/*
 * Random set of variables used by more than one routine.
 */
struct inode *rootdir;		/* pointer to inode of root directory */
struct proc *runq;		/* head of linked list of running processes */
short	cputype;		/* type of cpu = 40, 45, 70, 780 */
time_t	lbolt;			/* time in HZ since last boot */
time_t	time;			/* time in sec from 1970 */

int	mpid;			/* generic for unique process id's */
char	runin;			/* scheduling flag */
char	runout;			/* scheduling flag */
char	runrun;			/* scheduling flag */
char	curpri;			/* more scheduling */
int	maxmem;			/* actual max memory per process */
#ifdef pdp11
physadr	lks;			/* pointer to clock device */
#endif
int	physmem;		/* physical memory on this CPU */
daddr_t	swplo;			/* block number of swap space */
int	nswap;			/* size of swap space */
int	updlock;		/* lock for sync */
daddr_t	rablock;		/* block to be read ahead */
#ifdef pdp11
char	regloc[];		/* locs of saved user registers (trap.c) */
#endif
dev_t	rootdev;		/* device of the root */
dev_t	swapdev;		/* swapping device */
dev_t	pipedev;		/* pipe device */
int	icode[];		/* user init code */
int	szicode;		/* its size */
int	blkacty;		/* active block devices */
int	pwr_cnt, pwr_act;
int	(*pwr_clr[])();
dev_t getmdev();
daddr_t	bmap();
struct inode *ialloc();
struct inode *iget();
struct inode *owner();
struct inode *maknode();
struct inode *namei();
struct buf *alloc();
struct buf *getblk();
#ifdef vax
struct buf *geteblk();
#else
struct buf *getablk();
#endif
struct buf *bread();
struct buf *breada();
struct filsys *getfs();
struct file *getf();
struct file *falloc();
int	uchar();

/*
 * Structure of the system-entry table
 */
extern struct sysent {
	char	sy_narg;		/* total number of arguments */
	char	sy_nrarg;		/* number of args in registers */
	int	(*sy_call)();		/* handler */
} sysent[];
