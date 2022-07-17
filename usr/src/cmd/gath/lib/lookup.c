#define NLIN 8
#define NLIX 600

struct lins
{
	char *hst,*sys,*dir,*pfx,*dev,*pno;
};

lookup(pf,dr,dv,pn)
char *pf,*dr,*dv,*pn;
{
	struct lins *q,*qx,line[NLIN];
	char *p,bf[40],linx[NLIX];
	register int t;
	t=rjetab("lines",line,NLIN*6,linx,NLIX);
	qx=(q=line)+(t/6);
	p=(char *)rjesys();
	for (;q<qx;q++)
	{
		if (scan(pf,q->pfx)==0 || scan(q->pfx,pf)==0) continue;
		if (*p==(*q->sys)) break;
		if (scan(p,q->sys)==0) continue;
		cat(bf,q->dir,"/pool/...");
		if ((t=creat(bf,0666))<0) continue;
		close(t); 
		break;
	};
	if (q>=qx) return (-1);
	strcpy(dr,q->dir);
	strcpy(dv,q->dev);
	strcpy(pn,q->pno);
	return (0);
};

