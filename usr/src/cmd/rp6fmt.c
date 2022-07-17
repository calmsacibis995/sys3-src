#include <stdio.h>
#include <sys/types.h>
#define RP06_0
#include <sys/io.h>
#define RP6CYL 815	/* no. RP06 cylinders/pack */
#define RP6TRK 19	/* no. tracks/cyl */
#define RP6SEC 22	/* no. sectors/track */
#define RP6ST (RP6SEC*RP6TRK)	/* no. sectors/cyl */
#define MAXSEC (RP6CYL*RP6TRK*RP6SEC)	/* sectors/pack */
#define RPHEAD 8	/* no. sector header bytes supplied by user */
#define M_BASE 0x20010000	/* phys addr MBA 0 */
#define MBAinit 01	/* MBA init bit */
#define MB_ERR 0x3080	/* expected error bits in MBA status reg */
#define RP_GO 1		/* go bit */
#define RP_OFF 014	/* head offset */
#define RP_RTC 016	/* Return to Centerline */
#define RP_RED 070	/* Read */
#define RP_RHD 072	/* RP06 read sector header and date */
#define RP_WHD 062	/* RP06 write sector header and data */
#define RP_WCH 052	/* Write Check Header and Data */
#define RP_DC 010	/* drive clear */
#define RP_FMT 0x1000	/* format bit for offset reg */
#define RP_RIP 0x10	/* Read-in Preset RP06 */
#define RP_DRY 0x80	/* drive ready, status reg */
#define RP_ERR 0x4000	/* composite error, status reg */
#define RP_MOL 0x1000	/* medium-online bit in status reg */
#define RP_WRL 0x800	/* write lock status */
#define RP_ECH 0x40	/* ECC Hard Error */
#define RP_DCK 0x8000	/* ECC */
#define RP_HCRC 0x100	/* header crc err */
#define RP_HCE 0x80	/* header compare err */
#define RP_FER 0x10	/* format err */
#define RP_PAR 0x8	/* parity err */
#define RP_HCI 0x400	/* Header Compare Inhibit, offset reg */
#define RP_ECI 0x800	/* ECC Inhibit, offset reg	*/
#define M_WCKU 0x400	/* Write Check Upper error status - MBA */
#define M_WCKL 0x200	/* Write Check Lower error status - MBA */
#define BLKSIZ 512
#define BUFSZ ((RPHEAD + BLKSIZ) * RP6SEC)
#define MAXUNI 8	/* max. no. units on MBA */
#define MAXMBA 4	/* no. MBA's on system */
#define MAXERR 50
#define PTRN1 0xeb6d	/* DEC defined */
#define PTRN2 0xb6db	/* worst case pattern */
#define OBOFF (((int)obuf) & 0x1ff)
#define IBOFF (((int)ibuf) & 0x1ff)
#define IBMAP0 (128 << 9)
int Report = 0;
union MBREGS {
	struct MBAR {
		int M_csr,	 M_cr,	 M_sr,
		M_var, 	M_bc;
	}Mbar;
		struct {char fill1[0x400];
		struct RPR {
			int	RP_cr, RP_ds, RP_er1, RP_mr;
			int	RP_as, RP_da, RP_dt, RP_la;
			int	RP_sn, RP_off, RP_cyl, RP_cc;
			int	RP_err2, RP_err3, RP_Epos, RP_Epat;
			int	fill2[16];
		} RPreg[8];
	}Unit;
	struct MAP {long fill3[0x200], M_map[256];}Map;
} *Mptr;
struct sect {
	short s_cyl[1];
	char  s_sec;
	char  s_trk;
	short s_kw1, s_kw2;
	short s_data[256];
}; 
int track, cylndr, secnum, mnum, dunit, ecount, daterr, wcerr;
char wcrflg;
char *ibuf, *obuf;	/* buffer pointers */
short fmtflg;
int Offset[]	/* Centerline offsets */
	= {0, 0x08, 0x88, 0x10, 0x90, 0x18, 0x98, -1
}; 
struct EL {
	int e_typ, e_cyl, e_trk, e_err;
} errlst[MAXERR];
struct RPR *RPptr,Test;
extern char *malloc();
extern char *errtyp();
	/*
	* Stand-alone program to format RP06 disk.
	* User specifies disk unit.
	*/
main() 
{
	int i, j;
	char c;
	printf("rp6fmt: Format RP06 Disk\n");
#ifndef TEST
	if(ckcomp()) return;
#endif
	obuf = malloc(BUFSZ);
	ibuf = malloc(BUFSZ);
	dunit = 0;
	mnum = 0;
#ifndef TEST
	obinit(mnum);	/* setup map registers and output buffer */
#endif
	ungetc('v',stdin);	/* print vocabulary */
	while((c = getc(stdin)) != 'q'){
		switch ( c ) {
	case 'm':
		if ((getnum(&mnum)) == 0 || mnum < 0 || mnum >= MAXMBA){
			printf("need MBA # between 0 and %d\n",
				MAXMBA - 1);
			tossin(1);
			mnum = 0;	/* reset to default */
		}
		obinit(mnum);
		break;
	case 'd':
		if ((getnum(&dunit)) == 0 || dunit <= 0 || dunit >= MAXUNI){
			printf("need unit # between 1 and %d\n",MAXUNI - 1);
			tossin(1);
			dunit = 0;	/* reset */
		}
		break;
	case 'f':
		if (dunit == 0){
			printf("\nmust give a unit #\n");
			break;
		}
		printf("\n *** THE PACK ON DRIVE %d-%d",
			mnum,dunit);
		printf(" WILL BE OVER-WRITTEN DURING FORMATTING ***\n");
		tossin(0);
		printf("\nDO YOU WISH TO CONTINUE? (y or n) ");
		if ((c = getc(stdin)) != 'y'){
			printf("*** FORMAT CANCELLED ***");
			if(c == '\n') ungetc(c,stdin);
			break;
		}
		tossin(1);
		fmtflg++;
		if (pkinit()) {
			printf("init error\n");
			break;
		}
	
		RPptr->RP_off = RP_FMT | RP_ECI; /* format and ECC inhibit */
		printf("\n\n *** FORMATTING PACK ***\n\n");
		ecnt0();
		for (cylndr = 0; cylndr < RP6CYL; cylndr++) {
			for (track = 0; track < RP6TRK; track++) {
				if (format(OBOFF,BUFSZ,0)) break;
				if (wcheck(OBOFF,BUFSZ,0)) break;
				trkinc();
			}
			cylinc();
			trkclr();
			if(Report && ((cylndr % 50) == 0))
				printf("\n---cyl  %d---\n",cylndr);
		}
	
		esum();
		if(Report) edump();
		printf("\n\n*** FORMAT COMPLETE *** \n\n");
		printf("\n\n");
	case 'c':
		if (dunit == 0){
			printf("\nmust give a unit #\n");
			break;
		}
		printf("\n*** checking pack on drive %d-%d ***\n",mnum,dunit);
		tossin(1);
		ecnt0();
		fmtflg = 0;
		if(pkinit()) {
			printf("init error\n");
			break;
		}
		RPptr->RP_off = RPptr->RP_off | RP_FMT;
		for (cylndr = 0; cylndr < RP6CYL; cylndr++) {
			for (track = 0; track < RP6TRK; track++) {
				if (dread(IBOFF,BLKSIZ * RP6SEC,0)) break;
				trkinc();
			}
			cylinc();
			trkclr();
			if(Report && ((cylndr % 50) == 0))
				printf("\n---cyl  %d---\n",cylndr);
		}
		esum();
		if(Report) edump();
		printf("\n\n*** CHECK COMPLETE *** \n\n");
		break;
	case 'b':
		if(getnum(&i) == 0) {
			printf(" no bits set\n");
			tossin(1);
			break;
		}
		prbits(i);
		break;
	case 'X':
		whoa (Report);
		break;
	case 'R':
	case 'T':
		if(getnum(&Report) == 0) {
			printf("\ninvalid Report level; ");
			Report = 0;
		}
		printf("\nReport level = %d\n",Report);
		break;
	case 'v':
		printf("\nCommands:\n\tm n - MBA number, ");
		printf("0<=n<=%d (defalts to 0)",MAXMBA - 1);
		printf("\n\td n - Drive number, ");
		printf("1<=n<=%d (must pick one)",MAXUNI - 1);
		printf("\n\tf   - format pack");
		printf("\n\tc   - check pack format");
		printf("\n\tRn  - set Report level to n");
		printf("\n\tbn  - report bits set in n");
		printf("\n\tX   - to execute the report trap ");
		printf("\n\tq   - quit");
		printf("\n\tv   - print vocabulary");
	case '\n':
		printf("\n? ");
	case ' ':
		break;
	case 'Z':
		RPptr = &Test;
		ecount = 0;
		printf("\n number of errors ? ");
		getnum(&j);
		for(i = 0; i < j; i++) {
			RPptr->RP_cyl = 10 * i;
			RPptr->RP_da = (i + (256 * i)) & 0x1f1f;
			if(logerr(i%10 + 1,0x12345)) break;
		}
		esum();
		edump();
		break;
	default:
		ungetc(c,stdin);
		printf("\n dont know how to '");
		while((c = getc(stdin)) != '\n') printf("%c",c);
		printf("'.\n? ");
		}
	}
}
getnum(num)
int *num;
{
	char c;
	while((c =getc(stdin)) == ' ' || c == '\n');
	ungetc(c,stdin);
	if ((c = getc(stdin)) != '0') {
		ungetc(c,stdin);
		if (scanf("%d",num) == 0) return(0);
		return(1);
	}
	if ((c = getc(stdin)) != 'x') {
		ungetc(c,stdin);	/* may be a number, may not. */
		ungetc('0',stdin); /* leading 0 be only # */
		if (scanf("%o",num) == 0) return(0);
	}
	else {
		if (scanf("%x",num) == 0) return(0);
	}
	return(1);
}
tossin(n)
int n;
{
	while(getc(stdin) != '\n');
	if (n) ungetc('\n',stdin);
	return;
}
	/*
	*	reset error counters
	*/
ecnt0()
{
	ecount = daterr = wcerr = 0;
}
 
	/*
	* Initialize MBA (disk) .
	* Set up MBA map register to map a 
	* output buffer uses 1st 128 map reg's -
	* input buffer uses last 128 map reg's 
	*/
obinit(mnum)
int mnum;
{
	register int page, *mp0, i;
	Mptr =(union MBREGS *)(M_BASE + (mnum*0x2000));
	page = (int)((int)obuf>>9) & 0x1fffff;
	mp0 = Mptr->Map.M_map;
	for (i = 0; i < 128; i++)
		(*mp0++) = 0x80000000 | page++;
 
	page = (int)((int)ibuf>>9) & 0x1fffff;
	for (i = 0; i<128; i++ )
		(*mp0++) = 0x80000000 | page++;
	return;
}
	/*
	* Pack initialization.
	* transfer of 'BUFSZ' bytes.
	*/
pkinit() 
{
	int stat, i, j;
	register struct sect *secp;
	Mptr->Mbar.M_cr = MBAinit;	/* MBA init */
	RPptr = &Mptr->Unit.RPreg[dunit];
	if (((stat = (RPptr->RP_ds)) & RP_MOL) == 0) {
		printf("unit OFF-LINE\n");
		return(-1);
	}
	if(fmtflg){
		if(stat & RP_WRL){
			printf("unit WRITE-PROTECTED\n");
			return(1);
		}
		/*
		*  initialize output buffer with sector header and data:
		* 	4 sector header words:
		* 		cylinder
		* 		track/sector
		* 		key word 1
		* 		key word 2
		* 	256 data words
		*/
		for (secp = (struct sect *)obuf,i = 0;i<RP6SEC;i++,secp++) {
			secp->s_cyl[0] = RP_FMT;
			secp->s_kw1 = secp->s_kw2 = 0;
			secp->s_sec = i;	
			secp->s_trk = 0;	
			for (j = 0; j < 256;){
				secp->s_data[j++] = PTRN1;
				secp->s_data[j++] = PTRN2;
			}
		}
	}
	RPptr->RP_cr = RP_RIP | RP_GO; /* drive preset - sets vv */
	dwait(RPptr);
	ecount = daterr = wcerr = 0;
	return(0);
}
	/*
	* write 'nb' bytes worth of sector and data info
	* from buffer whose map/offset is 'baddr' to disk.
	*/
format(baddr,nb,s)
int baddr, nb, s;
{
	register int i;
	register struct RPR *R;
	register struct MBAR *M;
	R = RPptr;
	M = Mptr;
	R->RP_cr = RP_DC | RP_GO;
	dwait(R);
	setadr(baddr,nb,s);
	R->RP_cr = RP_WHD | RP_GO; /* write sector header and data */
	dwait(R);	
	if (i = derror(R)) {
		cursec(nb,s);
		logerr(3,i);
		if (wcrflg) return(1);	/* stop if 'wrt check' rec. */
		if (ecount > MAXERR) return(-1);
	}
	if (i = mbaerr(M)) {
		cursec(nb,s);
		if((wcrflg == 0) || logerr(1,i)) return(-1);
	}
	return(0);	
}
	/*
	* With ECC enabled,
	* read 'nb' bytes into buffer pointed to
	* by map 'baddr' - start at sector 's'.
	*/
dread(baddr,nb,s)
int baddr, nb, s;
{
	register int i;
	register struct MBAR *M;
	register struct RPR *R;
	R = RPptr;
	M = Mptr;
	setadr(baddr,nb,s);
rini:
	R->RP_cr = RP_DC | RP_GO;	
	dwait(R);
	dwait(R);
	R->RP_cr = RP_RED | RP_GO;	
	dwait(R);	
	cursec(nb,s);
	if (i = derror(R)) {	
		if (wcrflg) return(-1);
		if (i & (RP_HCE|RP_HCRC|RP_FER|RP_PAR)){
			logerr(14,i);
			return(1);
		}
		if (i & RP_DCK) {	
			daterr++;
			if (i & RP_ECH) {	/* ECC Hard Error */
				if(logerr(5,R->RP_Epat)
					|| wcrflg) return(-1);
			}
			else {		/* ECC recoverable */
				logerr(8,R->RP_Epat & 0xfff);
			}
			if (M->M_bc) {	/* more i-o to complete */
				goto rini;
				/* status reg cleared by Drive Clear */
			}
		}
		else {
			if( i & RP_FER)
				logerr(9,i);
			else
				logerr(4,i);
		}
		if (wcrflg) return(0);
	}
	if (i = mbaerr(M)) {
		return(-1);
	}
	return(0);
}
mbaerr(mba)
register struct MBAR *mba;
{
	register int i;
	if ((i = mba->M_sr) == 0x2000 || i == MB_ERR) return(0); 
	mba->M_sr = (-1);
	return(i);
}
	/*
	* write check 'nb' bytes of sector and data info
	* from buffer whose map reg is 'baddr' - start at sector 's'.
	*/
wcheck(baddr,nb,s)
int baddr, nb, s;
{
	register int i, j;
	register struct RPR *R;
	register struct MBAR *M;
	R = RPptr;
	M = Mptr;
	R->RP_cr = RP_DC | RP_GO;
	dwait(RPptr);
	setadr(baddr,nb,s);
	R->RP_cr = RP_WCH | RP_GO; 
	dwait(RPptr); 
	j = cursec(nb,s);
	if (i = derror(R))  {
		if(R->RP_er1 & RP_DCK) {
			logerr(7,i);
			R->RP_cr = RP_RIP | RP_GO;	/* resets ECC inhibit */
			R->RP_off = R->RP_off | RP_FMT;
			if((j = dread(IBOFF + IBMAP0,RPHEAD + BLKSIZ,j)) == 0)
				logerr(8,R->RP_Epat & 0xfff);
			R->RP_off = R->RP_off | RP_ECI | RP_FMT;
			return(i);
		}
	}
	else if (i = mbaerr(M)) {
		if (i & (M_WCKU | M_WCKL)) { 
			wcerr++;
			if(wcrflg) {
				return(1);
			}
		wckrcv(j);
		}
	}
	if (ecount > MAXERR) return(-1);
	return(0);
}
	/*
	* Try to recover from a 'write check' error during a
	* 'Write Check Header and Data' function.
	* 'RPptr' is ptr to RP register set.
	* 'Mptr' is ptr to MBA reg set.
	* MBA byte count reg has neg. no. bytes remaining - transfer 
	* stops on error, even in middle of a sector.
	* Loop with a 'Write Sector Header and Data' followed by a 
	* 'Read Sector Header and Data' - each loop iteration uses 
	* a different head centerline offset.
	* If all fails, report irrecoverable error and finish off the
	* 'write check data and header' on the rest of the track.
	*/
wckrcv(j)
int j;
{
	register int i, k, l, m;
	wcrflg++;
	m = j * (BLKSIZ+RPHEAD);	/* data offset in buffer */
	/* See if a simple reread (3) will fix this problem */
	for(l = 1; l <= 16; l++){
		if(wcheck(m + OBOFF,BLKSIZ + RPHEAD,j)) continue;
		logerr(12,l);
		goto wckfin;
	}
	/*
	* Try to recover using 
	* different centerline offset - 1st offset is 0.
	*/
	for (i = 0; (l = Offset[i]) >= 0; i++) {
		k = RPptr->RP_off & 0xff00;
		RPptr->RP_off = k | l;
		RPptr->RP_cr = RP_DC | RP_GO;
		dwait(RPptr);
		RPptr->RP_cr = RP_OFF | RP_GO;
		dwait(RPptr);
		if (format(m + OBOFF,BLKSIZ + RPHEAD,j)) continue;
		if (rshd(IBMAP0 + IBOFF,BLKSIZ + RPHEAD,j) < 0) continue;
		if (cksec(ibuf,obuf+m)) continue;
		secnum = j;
		logerr(6,(i << 12) | l);
		goto wckfin;
	}
	secnum = j;
	logerr(10,RPptr->RP_da);
wckfin :
	RPptr->RP_cr = RP_RTC | RP_GO; /* return to centerline */
	dwait(RPptr);
	/*
	* Continue write check where
	* the original WCHD left off.
	*/
	if(++j >= RP6SEC)  {
		wcrflg = 0;
		return;
	}
	i = j * (BLKSIZ + RPHEAD);
	if(wcheck(i + OBOFF,(RP6SEC - j) * (BLKSIZ + RPHEAD),j))
				logerr(13,i);
	wcrflg = 0;
	return;
}
	/*
	* read 'nb' bytes into buffer pointed to
	* by map and offset in 'baddr' - start at sector 's'
	*/
rshd(baddr,nb,s)
int baddr, nb, s;
{
	register int i, j;
	register struct MBAR *M;
	register struct RPR *R;
	R = RPptr;
	M = Mptr;
	setadr(baddr,nb,s);
	R->RP_cr = RP_DC | RP_GO; 
	dwait(R);
	R->RP_cr = RP_RHD | RP_GO; /* read sector header and data */
	dwait(R);	/* wait for i/o to finish */
	if (i = derror(R)) {	
		cursec(nb,s);
		if(i & (RP_HCRC|RP_HCE|RP_FER)){
			logerr(14,i);
			return(-1); 
		}
		if(R->RP_er1 & RP_DCK) {
			R->RP_cr = RP_RIP | RP_GO;	/* resets ECC inhibit */
			R->RP_off = R->RP_off | RP_FMT;
			if((j = dread(IBOFF + IBMAP0,RPHEAD + BLKSIZ,s)) == 0)
				logerr(8,R->RP_Epat & 0xfff);
			R->RP_off = R->RP_off | RP_ECI | RP_FMT;
			return(i);
		}
		else logerr(2,i);
		return(i);
	}
	if (i = mbaerr(M)) {
		return(-1);
	}
	return(i);
}
	/* setup data transfer addresses */
setadr(baddr,nb,s)
int baddr, nb, s;
{
	RPptr->RP_cyl = cylndr;	
	RPptr->RP_da = (track<<8) | s;	
	Mptr->Mbar.M_bc = (-nb);
	Mptr->Mbar.M_var = baddr; /* virt addr reg = map no. + byte off */
}
	/*
	* Compare sector header and data info on disk against that
	* which was written from output buffer
	*/
cksec(insec,ousec)
register struct sect *insec, *ousec;
{
	register int j;
	for (j = 0; j < (RPHEAD>>1); j++)
		if (insec->s_cyl[j] != ousec->s_cyl[j])
			return(1);
	for (j = 0; j < 256; j++)
		if (insec->s_data[j] != ousec->s_data[j])
			return(1);
	return(0);
}
	/*
	* wait RP06 disc unit to be ready.
	*/
dwait(rptr)
struct RPR *rptr;
{
	while ((rptr->RP_ds & RP_DRY) == 0);
}
	/*
	* check for RP06 error.
	*/
derror(rptr)
struct RPR *rptr;
{
	if (rptr->RP_ds & RP_ERR) 
		return(rptr->RP_er1 & 0xffff);
	return(0);
}
	/*
	* given sector and count for last transfer, determine
	* the last partial sector transferred.
	*/
cursec(nb,s)
int nb,s;
{
	register int i, j;
	if((i = Mptr->Mbar.M_bc >> 16) == 0){
		secnum = (RPptr->RP_da & 0x1f) -1;
	}
	else {
		j = nb + i;	/* bytes transferred */
		secnum = ((j - 1) /(RPHEAD + BLKSIZ)) + s;
	}
	if(secnum >= RP6SEC) {
		printf("\n*** sector conversion error");
		printf("\n*** M_bc %x, bytes %x, sector %x ***\n",
			i,j,secnum);
		whoa(2);
	}
	if(secnum < 0 ) secnum =  RP6SEC - 1;
	return(secnum);
}
	/*
	* modify track/sector no. in output buffer sector headers.
	*/
trkinc()
{
	register struct sect *sp;
	register int i;
	for (i = 0, sp = (struct sect *)obuf; i < RP6SEC; i++, sp++)
		sp->s_trk++;
}
cylinc() 
{
	register struct sect *sp;
	register int i;
	for (i = 0, sp = (struct sect *)obuf; i < RP6SEC; i++, sp++)
		sp->s_cyl[0]++;
}
trkclr()
{
	register struct sect *sp;
	register int i;
	for (i = 0, sp = (struct sect *)obuf; i < RP6SEC; i++, sp++)
		sp->s_trk = 0;
}
	/*
	*	check we were assembled standalone for the VAX
	*/
ckcomp()
{
#ifndef vax
	printf("available only on the VAX 11/780\n");
#endif
#ifndef STANDALONE
	printf("available STAND-ALONE only \n");
	return(1);
#endif
#ifndef vax
	return(1);
#endif
	return(0);
}
	/*
	*	log errors in erlist
	*/
logerr(code,stat)
int code, stat;
{
	errlst[ecount].e_typ = code;
	errlst[ecount].e_cyl = RPptr->RP_cyl & 0xffff;
	errlst[ecount].e_trk = RPptr->RP_da;
	errlst[ecount].e_err = stat;
	if(secnum == (RP6SEC -1)){
		errlst[ecount].e_trk += ((-1) << 8);
	}
	prloc();
	printf(":  %s, %8x\n",errtyp(code),stat);
	if(++ecount > (MAXERR - 5)) {
		Report = 9;
		printf("\nerror count is %d of %d MAX\n",
			ecount,MAXERR);
	}
	if(Report != 8) whoa(Report);
	if(ecount >= MAXERR) return(-1);
	return(0);
}
	/*
	*	print bits set in a word
	*/
prbits(wrd)
int wrd;
{
	int j,k;
	printf("\nerror bits : ");
	for(j = 0, k = 1; j <= 31; j++, k <<= 1)
		if(wrd & k) printf("%d, ",j);
	printf("\n\n");
	return;
}
	/*
	*	summarize errors by type and location
	*/
esum()
{
	int n, h, bw, badpk,badecc;
	register int slice;
	register struct EL *elptr;
	int ernum, endcyl, ocyl, otrk, osec;
	int *hitcyl, hittrk[RP6TRK], hitsec[RP6SEC],
		hcyl[8], htrk[8], hsec[8], hard[8], soft[8], xsec[8],
		headr[8], dta[8], read[8], write[8],
		minb[8],maxb[8];
	if(ecount == 0){
		printf("\nNO ERRORS\n");
		return;
	}
	printf("\n Total errors: %d",ecount);
	hitcyl = ibuf; /* scratch space */
	for(n = 0; n < RP6CYL; n++) *(hitcyl + n) = 0;
	for(n = 0; n < RP6TRK; n++) hittrk[n] = 0;
	for(n = 0; n < RP6SEC; n++) hitsec[n] = 0;
	for(n = 0;n < 8; n++){
		hcyl[n] = htrk[n] = hsec[n] = 0;
		hard[n] = soft[n] = xsec[n] = 0;
		headr[n] = dta[n] = read[n] = write[n] = 0;
		minb[n] = 13;	/* 12 is max field width */
		maxb[n] = 0;
	}
	ernum = badecc = badpk = 0;
	elptr = errlst;
	ocyl = otrk = osec = -1;
	for(slice = 0; slice < 8; slice++) {
		if((endcyl = hp_sizes[slice + 1].cyloff) == 0)
			endcyl = RP6CYL;
		while((ernum < ecount) && (elptr->e_cyl < endcyl)) {
			if((n = elptr->e_cyl) < RP6CYL){
				(*(hitcyl + n))++;
				if(n != ocyl){
					hcyl[slice]++;
					ocyl = n;
					otrk = osec = -1;
				}
			}
			else printf("\nbad cylinder data %x\n",
				elptr->e_cyl);
			if((n = (elptr->e_trk >> 8) & 0x1f) < RP6TRK){
				hittrk[n]++;
				if(n != otrk){
					htrk[slice]++;
					otrk = n;
					osec = -1;
				}
			}
			else printf("\nbad track data %x\n",
				elptr->e_trk);
			if((n = (elptr->e_trk & 0x1f)) < RP6SEC){
				(*(hitsec + n))++;
				if(n != osec){
					hsec[slice]++;
					osec = n;
				}
			}
			else printf("\nbad sector data %x\n",
				elptr->e_trk);
			switch (elptr->e_typ) {
			case 2:
			case 6:
				read[slice]++;
				headr[slice]++;
				break;
			case 3:
				write[slice]++;
				headr[slice]++;
				break;
			case 4:
				read[slice]++;
				dta[slice]++;
				break;
			case 5:
				hard[slice]++;
				badpk++;
				break;
			case 7:
				write[slice]++;
				dta[slice]++;
				break;
			case 8:
				soft[slice]++;
				if((bw = brstw(&elptr->e_err)) > maxb[slice])
					maxb[slice] = bw;
				if((bw < minb[slice]) && bw)
					minb[slice] = bw;
				break;
			case 9:
				break;
			case 10:
			case 13:
				hard[slice]++;
				xsec[slice]++;
				break;
			case 12:
				soft[slice]++;
				break;
			}
			ernum++;
			elptr++;
		}
		printf("\nDisk slice %d error summary",slice);
		if(hard[slice]){
			printf("\n\n\t%d\tHard errors",hard[slice]);
		}
		if(soft[slice]){
			printf("\n\t%d\tRecoverable errors",soft[slice]);
			if(!fmtflg)
			printf("\n\t\tmin burst : %d, max burst : %d",
				minb[slice] % 13, maxb[slice]);
			if(maxb[slice] > 8) badecc++;
		}
		if(hcyl[slice]){
			printf("\n\t%d\tBad cylinders",hcyl[slice]);
		}
		if(htrk[slice]){
			printf("\n\t%d\tBad tracks",htrk[slice]);
		}
		if(hsec[slice]){
			printf("\n\t%d\tBad sectors",hsec[slice]);
		}
		if(write[slice]){
			printf("\n\t%d\tWrite errors",write[slice]);
		}
		if(read[slice]){
			printf("\n\t%d\tRead errors",read[slice]);
		}
		if(headr[slice]){
			printf("\n\t%d\tHeader errors",headr[slice]);
		}
		if(dta[slice]){
			printf("\n\t%d\tData errors",dta[slice]);
		}
	
	}
	if(badecc){
		printf("\n\n*** This pack has %d ECC error",badecc);
		if(badecc > 1) printf("s");
		printf(" That FAIL");
		if(badecc < 2) printf("S");
		printf(" new pack requirements ***\n");
		printf("*** If this is a new pack, ");
		printf("return it to the vendor ***\n\n");
	}
	if(badpk && !fmtflg){
		printf("\n\n*** This pack has %d HARD error",badpk);
		if(badpk > 1) printf("s");
		printf(". It is UNUSABLE as a system pack ***\n\n");
		printf("*** This pack must be refurbished ***\n\n");
	}
	printf("\n\nPack summary\n");
	printf("\n\nCylinder hits\n");
	for(n = 0;n < RP6CYL; n++){
		if(endcyl == RP6CYL) break;
		h = *(hitcyl + n);
		if(h){
			printf("\n %d:\t%d hit",n,h);
			if(h > 1) printf("s");
		}
	}
	printf("\n\nTrack hits\n");
	for(n = 0;n < RP6TRK; n++)
		if(hittrk[n]){
			printf("\n %d:\t%d hit",n,hittrk[n]);
			if(hittrk[n] > 1) printf("s");
		}
	printf("\n\nSector hits\n");
	for(n = 0;n < RP6SEC; n++)
		if(hitsec[n]){
			printf("\n %d:\t%d hit",n,hitsec[n]);
			if(hitsec[n] > 1) printf("s");
		}
}
brstw(eptr)
int *eptr;
{
	int i, j, b1, b2;
	b1 = b2 = 0;
	j = *eptr;
	for(i = 1;i < 13; i++){
		if(j % 2) {
			if( b1 == 0) b1 = i;
			b2 = i + 1;
		}
		j = j / 2;
	}
	return(b2 - b1);
}
	/*
	*	dump the error list
	*/
edump()
{
	int i;
	if(ecount == 0)	return;
	printf("\n\nerror file dump");
	printf("\n\nCYL\tTRK\tSECT\t  STATUS\t ERROR TYPE\n");
	for(i = 0; i < ecount; i++)
		printf("\n%3d\t%3d\t%3d\t%8x\t%s",
		errlst[i].e_cyl,
		(errlst[i].e_trk >> 8) & 0x1f,
		errlst[i].e_trk & 0x1f,
		errlst[i].e_err,
		errtyp(errlst[i].e_typ));
	printf("\n");
}
	/*
	*	print cyl-trk-sect address
	*/
prloc()
{
	printf("\ncyl %d, trk %d, sec %d",
		cylndr,track,secnum);
}
	/*
	*	decode error type given the code number
	*/
char *
errtyp(n)
int n;
{
	switch(n) {
	case 1: return("MBA error");
	case 2: return("Read SHD");
	case 3: return("Write SHD");
	case 4: return("Read error");
	case 5: return("ECC; Non-recoverable");
	case 6: return("Write Check; Recoverable with head offset");
	case 7: return("Data check");
	case 8: return("ECC; Recoverable");
	case 9: return("Format error");
	case 10: return("Bad sector");
	case 11: return("Wckrcv sect. compare");
	case 12: return("Write Check; Recoverable by re-read");
	case 13: return("Multiple wrt-cks on track");
	case 14: return("Header Compare error");
	default: return("undefined");
	}
}
	/* debugging trap */
whoa(n)
int n;
{
	int i, j, *mp;
	static int passn = 1;
	char c;
	switch(n){
case 0: return;
case 1:
	printf("\n\nMBA Regs:\
		\n csr  %9x\n cr   %9x\n sr   %9x\
		\n var  %9x\n bc   %9x\n",
		Mptr->Mbar.M_csr,
		Mptr->Mbar.M_cr,
		Mptr->Mbar.M_sr,
		Mptr->Mbar.M_var,
		Mptr->Mbar.M_bc);
	break;
case 2:
	printf("\n\nMBA Regs:\
		\n csr  %9x\n cr   %9x\n sr   %9x\
		\n var  %9x\n bc   %9x\n",
		Mptr->Mbar.M_csr,
		Mptr->Mbar.M_cr,
		Mptr->Mbar.M_sr,
		Mptr->Mbar.M_var,
		Mptr->Mbar.M_bc);
case 3:
	printf("\nDrive Regs:\
		\n cr   %5x\tds   %5x\ter1  %5x\tmr   %5x\
		\n as   %5x\tda   %5x\tdt   %5x\tla   %5x\
		\n sn   %5x\toff  %5x\tcyl  %5x\tcc   %5x\
		\n err2 %5x\terr3 %5x\tepos %5x\tepat %5x\n",
		RPptr->RP_cr & 0xffff,
		RPptr->RP_ds & 0xffff,
		RPptr->RP_er1 & 0xffff,
		RPptr->RP_mr & 0xffff,
		RPptr->RP_as & 0xffff,
		RPptr->RP_da & 0x1f1f,
		RPptr->RP_dt & 0xffff,
		RPptr->RP_la & 0xffff,
		RPptr->RP_sn & 0xffff,
		RPptr->RP_off & 0xffff,
		RPptr->RP_cyl & 0xffff,
		RPptr->RP_cc & 0xffff,
		RPptr->RP_err2 & 0xffff,
		RPptr->RP_err3 & 0xffff,
		RPptr->RP_Epos & 0xffff,
		RPptr->RP_Epat & 0xffff);
		prloc();
		break;
case 8:
	prloc();
	return;
case 9:
	printf("\n***");
	}
	while(c != 'R'){
		if(passn < 2){
			printf("\nType 'c' fo continue,");
			printf("\n     'Rn' to change this trap to level n.");
			printf("\n and '<ctl> p' and 'H' to halt the system..");
			printf("\n\nReport levels:");
			printf("\n 9 - trap, no printout");
			printf("\n 8 - pack address info, NO trap");
			printf("\n 3 - drive registers");
			printf("\n 2 - things above and MBA regs");
			printf("\n 1 - MBA regs only");
			printf("\n 0 - no trap, re-enable with 'Tn' command");
		}
		printf("\nContinue?...");
		passn++;
		tossin(0);
		if((c = getc(stdin)) == 'c') return;
	}
	if(getnum(&Report) == 0) {
		printf("\ninvalid level");
		Report = 0;
	}
	printf("\nReport level %d\n",Report);
	return;
}
