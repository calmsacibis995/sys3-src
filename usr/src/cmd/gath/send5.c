#
/* send5.c */


/* This information is proprietary and is the property of Bell
   Telephone Laboratories, Incorporated.  Its reproduction or
   disclosure to others, either orally or in writing, is pro-
   hibited without written permission of Bell Laboratories. */


#define IDMOD send5

#define IDSTR "~|^`send5.c 1.15 1/16/76

#include	"sys/param.h"
#include	"send.h"
#include	"sys/stat.h"
#include	"sys/dir.h"

long	min,atol();

qout(nn,s)
char *s;
{
	char *tnm,*px,*qx;
	static char *who;
	register int i;
	register char *p,*q;
	int n;
	qat=1;
	n = nn;
	if (who==0)
	{
		if (n<4) goto E;
		p=s; 
		q=ss;
		for(i=0;i<4;i++) *q++=0137&rtr[0377&*p++];
		if (mtch(4,"@RUN",ss))
		{
			px=(p=s)+n;
			if(n > 0)
			do
				*p = rtr[0377 & *p];
			while(++p < px);
			ascii(0);
			maxcol=132;
			px=(p=s)+n;
			if(n > 0)
			do
				*p = trt[0377 & *p];
			while(++p < px);
			who="uvac";
		};
		if (s[0]==SLS) who="hasp";
		if((s[0]&0377)==0311 && (s[1]&0377)==0304)
			who="hasp";
	hasp:
		p=s; 
		q=ss;
		*q++=rtr[0377&*p++];
		for (i=1;i<4;i++) *q++=0137&rtr[0377&*p++];
		for (;i<80;i++) *q++=0377&rtr[0377&*p++];
		q = &ss[4];
		if (mtch(4,"!JOB",ss)) {
			while(*q++ != ',') ;
			while(*q++ != ',') ;
			while(*q != ',' && *q != ' ' && *q != '\0') q++;
			if (*q != ',') {
				who = "??"; 
				goto E;
			};
			px=(p=s)+n;
			if(n > 0)
			do
				*p = rtr[0377 & *p];
			while(++p < px);
			ascii(1);
			maxcol=80;
			px=(p=s)+n;
			if(n > 0)
			do
				*p = trt[0377 & *p];
			while(++p < px);
			who = "sig5";
		}
		if (who==0) goto E;
		if ((tnm=(char *)site(who))==0) goto C;
		if ((i=mktmp(tnm,0))<0)
		{
			prf("Cannot create temporary %s.\n",tnm); 
			goto C;
		};
		tmpf=tnm;
		inibf(&qbf,i);
	};
C: 
	p=s;
	if ((*(p+2)==DOL && *p==SLS && *(p+1)==STR && code==BCD && n>=3)
	    /*
	     || (*(p+2)=='B' && *p==']' && *(p+1)=='R' && code==ASC && n>=3)
	*/
	)
	{
E: 
		return (-1);
	};
	if (tmpf)
	{
		cnt++; 
		cue=1;
		p=s+n;
		while (n>0 && *--p==trt[' ']) n--;
		px=(p=s)+n;
		q=qbf.bp; 
		qx=qbf.bf+512;
		*q++=n;
		while (p<px)
		{
			if (q>=qx)
			{
				qbf.bp=q; 
				q=(char *)flush(&qbf);
			};
			*q++=(*p++);
		};
		qbf.bp=q;
		if (q>=qx) flush(&qbf);
	};
	return (0);
};

struct lins
{
	char *hst,*sys,*dir,*pfx,*dev,*phne;
};

site(s)
char *s;
{
	static char qnm[40],tnm[40],hnm[40];
	struct lins *linz,line[NLIN];
	char *u,linx[NLIX];
	int n,t;
	register int i;
	register char *p;
	register struct lins *q;
	for (p=s;*p;p++);
	n=p-s;
	q=line;
	i=rjetab("lines",q,NLIN*6,linx,NLIX);
	linz=q+(i/6);
	u=(char *)rjesys();
	for (;q<linz;q++)
	{
		if (!mtch(n,s,q->pfx)) continue;
		for (i=0;i<NHST;i++)
		{
			if (nhst>=0 && i!=nhst) continue;
			if (mtch(host[i].hcnt,host[i].hnam,q->hst)) break;
		};
		if (i>=NHST) continue;
		p=q->sys;
		if (*u==0 || *u!=(*p))
		{
			while (*++p!=0) {
				if (*u==(*p)) break;
			};
			if (*u!=(*p)) continue;
			prf("%q%s/pool/...",tnm,q->dir);
			if ((t=creat(tnm,0666))<0) continue;
			close(t);
		};
		min = atol(q->phne);
		if (space(q->dir)<0) abt();
		qer=qnm;
		prf("%q%s/%sqer",qnm,q->dir,q->pfx);
		prf("%q%s",hnm,q->dir);
		home = hnm;
		prf("%q%s/pool/stm",tnm,q->dir);
		return ((int)tnm);
	};
	if(mtch(4,"sig5",s)) p = "SIGMA RJE";
	else if(mtch(4,"nlsc",s)) p = "CONNECTION TO NLSC";
	else p=(mtch(4,"hasp",s) ? "IBM RJE":"UNIVAC RJE");
	prf("UNIX System %s is not configured for %s",u,p);
	if (nhst>=0) prf(" to %s",host[nhst].hnam);
	prf(". (570)\n");
	return (0);
};

svdir(a,n)
int a[];
{
	struct	stat	sb;
	register int k,t,*p;
	if ((k=n-2)<=0) return (-1);
	stat(".",&sb); 
	t = sb.st_ino;
	p=a; 
	*p++ = sb.st_dev;
	while (t != ROOTINO)
	{
		k--; 
		*p++=t;
		stat("..",&sb); 
		t = sb.st_ino;
		if (t == ROOTINO) break;
		if (k<=0 || chdir("..")<0) break;
	};
	if (t != ROOTINO) t=0; 
	*p=t;
	return ((!t)|rsdir(a));
};

rsdir(a)
int a[];
{
	int *p0;
	struct	direct	db;
	struct	stat	sb;
	register int f,d,*p;
	char	name[DIRSIZ + 2];
	char	*s,*q;
	for (p = a+1;(*p != 0) && (*p != ROOTINO);p++); 
	p0=p;
	stat(".",&sb);
	if (sb.st_dev != *a)
	{
		if (*p0 != ROOTINO)  goto E;
		f=0;
		if ((d=open("/",0))<0) goto E;
		while (read(d,&db,sizeof db) == sizeof db)
		{
			if (db.d_ino == 0) continue;
			s = name;
			*s++ = '/';
			for(q = db.d_name;q < &db.d_name[DIRSIZ];)
				*s++ = *q++;
			*s++ = '\0';
			stat(name,&sb);
			if (sb.st_dev == *a) {
				f=1; 
				break;
			};
		};
		close(d);
		if (f==0 || chdir(name)<0) goto E;
	};
	for (;;)
	{
		for (p=p0;p>a;p--)
		{
			if (*p == sb.st_ino) break;
		};
		if (p>a) break;
		if ((sb.st_ino == ROOTINO) || (chdir("..") < 0)) goto E;
		stat(".",&sb);
	};
	for (p--;p>a;p--)
	{
		f=0;
		if ((d=open(".",0))<0) goto E;
		while (read(d,&db,sizeof db) == sizeof db)
		{
			if (db.d_ino == 0)
				continue;
			s = name;
			for(q = db.d_name;q < &db.d_name[DIRSIZ];)
				*s++ = *q++;
			*s++ = '\0';
			stat(name,&sb);
			if (sb.st_ino == (*p)) {
				f=1; 
				break;
			};
		};
		close(d);
		if (f==0 || chdir(name)<0) goto E;
	};
	return (0);
E: 
	chdir ("/"); 
	return (-1);
};

struct {
	long		s_tfree;
	unsigned	s_tinode;
	char		s_fname[6];
	char		s_fpack[6];
} usb;

space(s)
char *s;
{
	struct	stat	sb;
	register int t;
	register char *p;
	if (stat(s,&sb)<0 || ustat(sb.st_dev,&usb)<0) return (0);
	t = 0;
	if (usb.s_tfree < (min + min/5)) t |= 1;
	if (usb.s_tinode < 50) t |= 2;
	if (t==0) return (0);
	prf("Cannot send - only ");
	if (t&1) prf("%L blocks",usb.s_tfree);
	if (t==3) prf(", ");
	if (t&2) prf("%l inodes",usb.s_tinode);
	prf(" free on file system ");
	for (p=s;*p;prc(*p++))
	{
		if (p>s && *p=='/') break;
	};
	prf(".\n");
	return (-1);
};

/*end*/
