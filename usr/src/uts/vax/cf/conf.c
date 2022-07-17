/*
 *  Configuration information
 */


#define	RP06_0 1
#define	TU16_0 1
#define	CON_0 1
#define	MEMORY_0 1
#define	TTY_0 1
#define	ERRLOG_0 1

#define	NBUF	48
#define	NINODE	125
#define	NFILE	125
#define	NMOUNT	8
#define	SMAPSIZ	75
#define	NCALL	50
#define	NPROC	65
#define	NTEXT	40
#define	NCLIST	150
#define	NSABUF	0
#define	POWER	0
#define	MAXUP	25
#define	NHBUF	64

#include	"sys/param.h"
#include	"sys/io.h"
#include	"sys/space.h"
#include	"sys/conf.h"
#include	"sys/uba.h"

extern nodev(), nulldev();
extern hpopen(), hpclose(), hpread(), hpwrite(), hpstrategy();
extern struct iobuf hptab;
extern htopen(), htclose(), htread(), htwrite(), htstrategy();
extern struct iobuf httab;
extern conopen(), conclose(), conread(), conwrite(), conioctl();
extern mmread(), mmwrite();
extern syopen(), syread(), sywrite(), syioctl();
extern erropen(), errclose(), errread();

struct bdevsw bdevsw[] = {
/* 0*/	hpopen,	hpclose,	hpstrategy,	&hptab,
/* 1*/	htopen,	htclose,	htstrategy,	&httab,
};

struct cdevsw cdevsw[] = {
/* 0*/	conopen,	conclose,	conread,	conwrite,	conioctl,
/* 1*/	nodev, 	nodev, 	nodev, 	nodev, 	nodev,
/* 2*/	syopen,	nulldev,	syread,	sywrite,	syioctl,
/* 3*/	nulldev,	nulldev,	mmread,	mmwrite,	nodev, 
/* 4*/	hpopen,	hpclose,	hpread,	hpwrite,	nodev, 
/* 5*/	htopen,	htclose,	htread,	htwrite,	nodev, 
/* 6*/	nodev, 	nodev, 	nodev, 	nodev, 	nodev,
/* 7*/	nodev, 	nodev, 	nodev, 	nodev, 	nodev,
/* 8*/	erropen,	errclose,	errread,	nodev, 	nodev, 
};

int	bdevcnt = 2;
int	cdevcnt = 9;

dev_t	rootdev = makedev(0, 0);
dev_t	pipedev = makedev(0, 0);
dev_t	dumpdev = makedev(1, 0);
dev_t	swapdev = makedev(0, 0);
daddr_t	swplo = 8000;
int	nswap = 9000;




struct	tty	con_tty[1];




int	(*pwr_clr[])() = 
{
	(int (*)())0
};

int	(*dev_init[])() = 
{
	(int (*)())0
};
