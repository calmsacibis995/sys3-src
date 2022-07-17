#include "defs"

gentemp(t)
ptr t;
{
register ptr oldp;
register ptr p;
register ptr q;

/* search the temporary list for a matching type */
for(oldp = &tempvarlist ; p = oldp->nextp ; oldp = p)
	{
	q = p->datap;

	if( (q->vtype==t->vtype)&&(q->vtypep==t->vtypep)
	   && eqdim(q->vdim,t->vdim) )
		{
		oldp->nextp = p->nextp;
		break;
		}
	}

if(p == 0)
	{
	q = allexpblock();
	q->tag = TTEMP;
	q->subtype = t->subtype;
	q->vtype = t->vtype;
	q->vclass = t->vclass;
	if(t->vtypep)
		q->vtypep = cpexpr(t->vtypep);
	else
		q->vtypep = NULL;
	q->vdim = t->vdim;
	mkftnp(q);	/* assign fortran types */

	p = mkchain(q,0);
	p->datap = q;
	}

p->nextp = thisexec->temps;
thisexec->temps = p;

return( cpexpr(q) );
/* need a copy of the block for the temporary list and another for use */
}


gent(t,tp)  /* make a temporary of type t, typepointer tp */
int t;
ptr tp;
{
static struct varblock model;

model.vtype = t;
model.vtypep = tp;

return( gentemp(&model) );
}
