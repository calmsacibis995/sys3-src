#define	NINST	100
#define	NSYM	500
#define	NHASH	(NSYM+NINST)
#define	NCPS	8
#define	NKMCI	1024
#define	NKMCD	1024

/*
 * Symbol types
 */
#define	XUNDEF	0x0
#define	XABS	0x1000
#define	XTEXT	0x2000
#define	XDATA	0x3000
#define	XREG	0x4000
#define	XTAG	0x8000
#define	XTYPE	0x7000

#define	DSTEP	1
#define	DSTOP	4
#define	DRESET	5
#define	DRUN	6
#define	DREGS	10
#define	DLOAD	11
#define	DDUMP	12

#define	KSTEP	1
#define	KMS	2
#define	KCSR	3
#define	KSTOP	4
#define	KMCLR	5
#define	KRUN	6
#define	KLU	7

#define	TBRG	01
#define	TMAR	06

#define	RLCMV	0
#define	RLCPOP	020000
#define	RLCPG	040000
#define	RLCBR	060000
#define	RLCMASK	070000
#define	RLUSE	0100000
#define	RLBUP	077777
#define	RLNEXT	07777

#define	ERR	(-1)

#define	SRCREGL	(1<<13)
#define	SRCMEM	(2<<13)
#define	SRCBRG	(3<<13)
#define	SRCREGH	(5<<13)

#define	SELA	(8<<4)
#define	SELB	(9<<4)

#define	DSTBRG	(1<<8)
#define	DSTREGH	(2<<8)
#define	DSTBGRS	(3<<8)
#define	DSTREGL	(4<<8)
#define	DSTMEM	(5<<8)
#define	DSTREG	(6<<8)
#define	DSTBREG	(7<<8)
#define	DSTMARP	(1<<11)
#define	DSTMAR	(2<<11)
#define	DSTMARI	(3<<11)

struct	symtab {
	char	name[NCPS];
	int	type;
	int	value;
};

struct	exp {
	int	xtype;
	int	xvalue;
};

struct	symtab	symtab[NSYM];
struct	symtab	*hshtab[NHASH];
struct	symtab	loclab[10];
struct	symtab	*dot;
int	lineno;
int	tsize;
int	dsize;
int	anyerrs;
int	textsv, datasv;
char	dbmode;
char	*outfile, *dbfile, *infile;
int	dbfd;
int	dbtab[3];
char	dbstr[50], instr[50];
struct	symtab	instab[];
short	ispace[NKMCI], reloc[NKMCI];
char	dspace[NKMCD];
struct	symtab	**lookup();

struct {
	char	csr[8];
	char	lur[8];
	char	reg[16];
	char	io[8];
	char	npr[2];
	char	brg;
	char	mem;
	char	fill[4];
} ir;
