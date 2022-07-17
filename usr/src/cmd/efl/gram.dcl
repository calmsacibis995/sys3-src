dcls1:	  dcl1
	| dcls1 EOS
	| dcls1 EOS dcl1
		{ $$ = hookup($1,$3); }
	;

dcl1:	  dcl
	| varlist
	;

dcl:	  attrs vars
		{ attvars($1,$2); $$ = $2; }
	| attrs LBRACK dcls1 RBRACK
		{ attvars($1,$3); $$ = $3; }
	| INITIAL initlist
		{ $$ = 0; }
	| IMPLICIT letton implist lettoff
		{ $$ = 0; }
	| EQUIVALENCE equivsets
		{ $$ = 0; }
	| EQUIVALENCE equivlist
		{ mkequiv($2); $$ = 0; }
	;

dcls:	  dcl
	| dcls EOS
	| dcls EOS dcl
		{ $$ = hookup($1,$3); }
	;

initlist:	init
	| initlist COMMA init
	;

init:	  lhs ASGNOP  {ininit = YES; }   expr
		= { ininit = NO;  mkinit($1,$4);  frexpr($1); }
	;

implist:  impgroup
	| implist COMMA impgroup;
	;

impgroup:  impspec
		{ setimpl(imptype, 'a', 'z'); }
	| impspec LPAR impsets RPAR
	;

impspec:  specs
		{ imptype = $1->attype; cfree($1); }
	;

impsets:  impset
	| impsets COMMA impset
	;

impset:	  LETTER
		{ setimpl(imptype, $1, $1); }
	| LETTER ADDOP LETTER
		{ setimpl(imptype, $1, $3); }
	;

equivsets:	equivset
	| equivsets COMMA equivset
	;

equivset:  LPAR equivlist RPAR
		{ mkequiv($2); }
	;

equivlist:  lhs COMMA lhs
		{ $$ = mkchain($1, mkchain($3,0)); }
	| equivlist COMMA lhs
		{ $$ = hookup($1, mkchain($3,0)); }
	;

attrs:	  attr
	| attrs attr	{ attatt($1,$2); }
	;

attr:	  spec dim	{ $1->atdim = $2; }
	| array dim	{ $$ = ALLOC(atblock); $$->atdim = $2; }
	;

dim:		{ $$ = 0; }
	| dimbound
	;

dimbound:  LPAR { inbound = 1; }  bounds RPAR
			{ inbound = 0;  $$ = arrays = mkchain($3,arrays); }
	;

bounds:	  bound
	| bounds COMMA bound	{ hookup($1,$3); }
	;

bound:	  ubound
		{
		$$ = ALLOC(dimblock);
		$$->lowerb = 0;
		$$->upperb = $1;
		}
	| expr COLON ubound
		{
		$$ = ALLOC(dimblock);
		$$->lowerb = $1;
		$$->upperb = $3;
		}
	;

ubound:	  expr
	| MULTOP  { $$ = 0; }
	;

vars:		{ $$ = 0; }
	| varlist
	;

varlist:  var
	| varlist COMMA var	{ hookup($1,$3); }
	;

var:	  varname dim
		{
		if($2!=0)
			if($1->vdim==0)
				$1->vdim = $2;
			else if(!eqdim($2,$1->vdim))
				dclerr("multiple dimension", $1->namep);
		$$ = mkchain($1,0);
		}
	| varname dim ASGNOP  { ininit = YES; }   expr
		{
		ininit = NO;
		if($3!=OPASGN)
			dclerr("illegal initialization operator", $1->sthead->namep);
		if($2!=0)
			if($1->vdim==0)
				$1->vdim = $2;
			else if(!eqdim($2,$1->vdim))
				dclerr("multiple dimension", $1->sthead->namep);
		if($5!=0 && $1->vinit!=0)
			dclerr("multiple initialization", $1->sthead->namep);
		$1->vinit = $5;
		$$ = mkchain($1,0);
		}
	;

varname:  NAME
		{ $$ = mkvar($1); }
	;


specs:	  specarray
	| specs specarray	{ attatt($1,$2); }
	;

specarray:  spec
	| array dimbound
		{ $$ = ALLOC(atblock); $$->atdim = $2; }
	;

spec:	  class contnu
		{
		$$ = ALLOC(atblock);
		if($1 == CLEXT)
			$$->atext = 1;
		else if($1>=0 && $1<=10)
			$$->atclass = $1;
		else	{
			$$->atclass = CLCOMMON;
			$$->atcommon = $1;
			}
		}
	| type
		{
		$$ = ALLOC(atblock);
		if($1 >=0 && $1<10)
			$$->attype = $1;
		else	{
			$$->attypep = $1;
			if($1->tag == TSTRUCT)
				$$->attype = TYSTRUCT;
			else if($1->tag == TFIELD)
				$$->attype = TYFIELD;
			else	$$->attype = TYCHAR;
			}
		}
	| prec
		{
		$$ = ALLOC(atblock);
		$$->atprec = $1;
		}
	;

class:	  AUTOMATIC	{ $$ = CLAUTO; fprintf(diagfile, "AUTOMATIC not yet implemented\n"); }
	| STATIC	{ $$ = CLSTAT; }
	| COMMON LPAR comneed comname RPAR
			{ $$ = $4; }
	| COMMON MULTOP comneed comname MULTOP
			{ $$ = $4; }
	| INTERNAL	{ $$ = CLSTAT; }
	| VALUE		{ $$ = CLVALUE; fprintf(diagfile, "VALUE not yet implemented\n");  }
	| EXTERNAL	{ $$ = CLEXT; }
	;

comneed:	{ comneed = 1; }
	;

comname:		{ $$ = mkcomm(""); }
	| COMNAME
	;

type:	  INTEGER	{ $$ = TYINT; }
	| REAL		{ $$ = TYREAL; }
	| COMPLEX	{ $$ = TYCOMPLEX; }
	| LOGICAL	{ $$ = TYLOG; }
	| DOUBLE PRECISION
			{ $$ = TYLREAL; /* holdover from Fortran */ }
	| DOUBLEPRECISION
			{ $$ = TYLREAL; /* holdover from Fortran */ }
	| CHARACTER LPAR expr RPAR
			{ $$ = $3; }
	| deftype
	| FIELD LPAR bound RPAR  { $$ = mkfield($3); }
	;

deftype:  STRUCTNAME
		{ $$ = $1->varp; }
	| STRUCT structname contnu struct
		  { $$ = mkstruct($2,$4); }
	| STRUCT struct
		{ $$ = mkstruct(0,$2); }
	;

structname:  NAME
		{ if($1->varp && $1->varp->blklevel<blklevel)
			hide($1);
		  $1->tag = TSTRUCT;
		}
	| STRUCTNAME
		{ if($1->varp)
			if($1->varp->blklevel<blklevel)
				hide($1);
			else dclerr("multiple declaration for type %s", $1->namep);
		}
	;

struct:	  LBRACK  { ++instruct; }   dcls  { --instruct; }   RBRACK EOS
			{ $$ = $3; prevv = -1; }
	;

array:	  ARRAY
	| DIMENSION
	;

prec:	  LONG	{ $$ = 1; }
	| SHORT	{ $$ = 0; }
	;
