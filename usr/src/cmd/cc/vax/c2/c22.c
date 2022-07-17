#include "c2.h"
char c22[] = "@(#)c22.c 1.9 80/01/02 16:43:35";
#define readonly

readonly char revbr[] = {
	JNE, JEQ, JGT, JLT, JGE, JLE,
	JNE, JEQ, JHI, JLO, JHIS, JLOS,
	JBS, JBC, JLBS, JLBC, JBSC, JBCC, JBSS, JBCS };

/* cursed be the preprocessor, whose impotence and stupidity
/* prevented this table being macro-generated from ../as/instrs
*/

readonly struct optab optab[] = {
"jbr",JBR,
"jeql",T(CBR,JEQ),
"jneq",T(CBR,JNE),
"jleq",T(CBR,JLE),
"jgeq",T(CBR,JGE),
"jlss",T(CBR,JLT),
"jgtr",T(CBR,JGT),
"jbc",T(CBR,JBC),
"jbs",T(CBR,JBS),
"jlequ",T(CBR,JLOS),
"jgequ",T(CBR,JHIS),
"jlssu",T(CBR,JLO),
"jgtru",T(CBR,JHI),
"jlbc",T(CBR,JLBC),
"jlbs",T(CBR,JLBS),
"jbcc",T(CBR,JBCC),
"jbsc",T(CBR,JBSC),
"jbcs",T(CBR,JBCS),
"jbss",T(CBR,JBSS),
"acbb",T(ACB,BYTE),
"acbd",T(ACB,DOUBLE),
"acbf",T(ACB,FLOAT),
"acbl",T(ACB,LONG),
"acbw",T(ACB,WORD),
"addb2",T(ADD,U(BYTE,OP2)),
"addb3",T(ADD,U(BYTE,OP3)),
"addd2",T(ADD,U(DOUBLE,OP2)),
"addd3",T(ADD,U(DOUBLE,OP3)),
"addf2",T(ADD,U(FLOAT,OP2)),
"addf3",T(ADD,U(FLOAT,OP3)),
"addl2",T(ADD,U(LONG,OP2)),
"addl3",T(ADD,U(LONG,OP3)),
"addw2",T(ADD,U(WORD,OP2)),
"addw3",T(ADD,U(WORD,OP3)),
"aobleq",AOBLEQ,
"aoblss",AOBLSS,
"ashl",T(ASH,LONG),
"ashq",T(ASH,QUAD),
"bbc",T(CBR,JBC),
"bbcc",T(CBR,JBCC),
"bbcci",T(CBR,JBCC),
"bbcs",T(CBR,JBCS),
"bbs",T(CBR,JBS),
"bbsc",T(CBR,JBSC),
"bbss",T(CBR,JBSS),
"bbssi",T(CBR,JBSS),
"bcc",T(CBR,JHIS),
"bcs",T(CBR,JLO),
"beql",T(CBR,JEQ),
"beqlu",T(CBR,JEQ),
"bgeq",T(CBR,JGE),
"bgequ",T(CBR,JHIS),
"bgtr",T(CBR,JGT),
"bgtru",T(CBR,JHI),
"bicb2",T(BIC,U(BYTE,OP2)),
"bicb3",T(BIC,U(BYTE,OP3)),
"bicl2",T(BIC,U(LONG,OP2)),
"bicl3",T(BIC,U(LONG,OP3)),
"bicw2",T(BIC,U(WORD,OP2)),
"bicw3",T(BIC,U(WORD,OP3)),
"bisb2",T(BIS,U(BYTE,OP2)),
"bisb3",T(BIS,U(BYTE,OP3)),
"bisl2",T(BIS,U(LONG,OP2)),
"bisl3",T(BIS,U(LONG,OP3)),
"bisw2",T(BIS,U(WORD,OP2)),
"bisw3",T(BIS,U(WORD,OP3)),
"bitb",T(BIT,BYTE),
"bitl",T(BIT,LONG),
"bitw",T(BIT,WORD),
"blbs",T(CBR,JLBS),
"blbc",T(CBR,JLBC),
"bleq",T(CBR,JLE),
"blequ",T(CBR,JLOS),
"blss",T(CBR,JLT),
"blssu",T(CBR,JLO),
"bneq",T(CBR,JNE),
"bnequ",T(CBR,JNE),
"brb",JBR,
"brw",JBR,
"bvc",T(CBR,0),
"bvs",T(CBR,0),
"callg",CALLS,
"calls",CALLS,
"caseb",T(CASE,BYTE),
"casel",T(CASE,LONG),
"casew",T(CASE,WORD),
"clrb",T(CLR,BYTE),
"clrd",T(CLR,DOUBLE),
"clrf",T(CLR,FLOAT),
"clrl",T(CLR,LONG),
"clrq",T(CLR,QUAD),
"clrw",T(CLR,WORD),
"cmpb",T(CMP,BYTE),
"cmpd",T(CMP,DOUBLE),
"cmpf",T(CMP,FLOAT),
"cmpl",T(CMP,LONG),
"cmpw",T(CMP,WORD),
"cvtbd",T(CVT,U(BYTE,DOUBLE)),
"cvtbf",T(CVT,U(BYTE,FLOAT)),
"cvtbl",T(CVT,U(BYTE,LONG)),
"cvtbw",T(CVT,U(BYTE,WORD)),
"cvtdb",T(CVT,U(DOUBLE,BYTE)),
"cvtdf",T(CVT,U(DOUBLE,FLOAT)),
"cvtdl",T(CVT,U(DOUBLE,LONG)),
"cvtdw",T(CVT,U(DOUBLE,WORD)),
"cvtfb",T(CVT,U(FLOAT,BYTE)),
"cvtfd",T(CVT,U(FLOAT,DOUBLE)),
"cvtfl",T(CVT,U(FLOAT,LONG)),
"cvtfw",T(CVT,U(FLOAT,WORD)),
"cvtlb",T(CVT,U(LONG,BYTE)),
"cvtld",T(CVT,U(LONG,DOUBLE)),
"cvtlf",T(CVT,U(LONG,FLOAT)),
"cvtlw",T(CVT,U(LONG,WORD)),
"cvtrdl",T(CVT,U(DOUBLE,LONG)),
"cvtrfl",T(CVT,U(FLOAT,LONG)),
"cvtwb",T(CVT,U(WORD,BYTE)),
"cvtwd",T(CVT,U(WORD,DOUBLE)),
"cvtwf",T(CVT,U(WORD,FLOAT)),
"cvtwl",T(CVT,U(WORD,LONG)),
"decb",T(DEC,BYTE),
"decl",T(DEC,LONG),
"decw",T(DEC,WORD),
"divb2",T(DIV,U(BYTE,OP2)),
"divb3",T(DIV,U(BYTE,OP3)),
"divd2",T(DIV,U(DOUBLE,OP2)),
"divd3",T(DIV,U(DOUBLE,OP3)),
"divf2",T(DIV,U(FLOAT,OP2)),
"divf3",T(DIV,U(FLOAT,OP3)),
"divl2",T(DIV,U(LONG,OP2)),
"divl3",T(DIV,U(LONG,OP3)),
"divw2",T(DIV,U(WORD,OP2)),
"divw3",T(DIV,U(WORD,OP3)),
"extv",T(EXTV,LONG),
"extzv",T(EXTZV,LONG),
"incb",T(INC,BYTE),
"incl",T(INC,LONG),
"incw",T(INC,WORD),
"insv",T(INSV,-1),
"jmp",JMP,
"mcomb",T(COM,BYTE),
"mcoml",T(COM,LONG),
"mcomw",T(COM,WORD),
"mnegb",T(NEG,BYTE),
"mnegd",T(NEG,DOUBLE),
"mnegf",T(NEG,FLOAT),
"mnegl",T(NEG,LONG),
"mnegw",T(NEG,WORD),
"movab",T(MOVA,BYTE),
"movad",T(MOVA,DOUBLE),
"movaf",T(MOVA,FLOAT),
"moval",T(MOVA,LONG),
"movaq",T(MOVA,QUAD),
"movaw",T(MOVA,WORD),
"movb",T(MOV,BYTE),
"movd",T(MOV,DOUBLE),
"movf",T(MOV,FLOAT),
"movl",T(MOV,LONG),
"movq",T(MOV,QUAD),
"movw",T(MOV,WORD),
"movzbl",T(MOVZ,U(BYTE,LONG)),
"movzbw",T(MOVZ,U(BYTE,WORD)),
"movzwl",T(MOVZ,U(WORD,LONG)),
"mulb2",T(MUL,U(BYTE,OP2)),
"mulb3",T(MUL,U(BYTE,OP3)),
"muld2",T(MUL,U(DOUBLE,OP2)),
"muld3",T(MUL,U(DOUBLE,OP3)),
"mulf2",T(MUL,U(FLOAT,OP2)),
"mulf3",T(MUL,U(FLOAT,OP3)),
"mull2",T(MUL,U(LONG,OP2)),
"mull3",T(MUL,U(LONG,OP3)),
"mulw2",T(MUL,U(WORD,OP2)),
"mulw3",T(MUL,U(WORD,OP3)),
"pushab",T(PUSHA,BYTE),
"pushad",T(PUSHA,DOUBLE),
"pushaf",T(PUSHA,FLOAT),
"pushal",T(PUSHA,LONG),
"pushaq",T(PUSHA,QUAD),
"pushaw",T(PUSHA,WORD),
"pushl",T(PUSH,LONG),
"ret",T(JBR,RET),
"sobgeq",SOBGEQ,
"sobgtr",SOBGTR,
"subb2",T(SUB,U(BYTE,OP2)),
"subb3",T(SUB,U(BYTE,OP3)),
"subd2",T(SUB,U(DOUBLE,OP2)),
"subd3",T(SUB,U(DOUBLE,OP3)),
"subf2",T(SUB,U(FLOAT,OP2)),
"subf3",T(SUB,U(FLOAT,OP3)),
"subl2",T(SUB,U(LONG,OP2)),
"subl3",T(SUB,U(LONG,OP3)),
"subw2",T(SUB,U(WORD,OP2)),
"subw3",T(SUB,U(WORD,OP3)),
"tstb",T(TST,BYTE),
"tstd",T(TST,DOUBLE),
"tstf",T(TST,FLOAT),
"tstl",T(TST,LONG),
"tstw",T(TST,WORD),
"xorb2",T(XOR,U(BYTE,OP2)),
"xorb3",T(XOR,U(BYTE,OP3)),
"xorl2",T(XOR,U(LONG,OP2)),
"xorl3",T(XOR,U(LONG,OP3)),
"xorw2",T(XOR,U(WORD,OP2)),
"xorw3",T(XOR,U(WORD,OP3)),
"movc3",MOVC3,
".globl",EROU,
".text",TEXT,
".data",DATA,
".bss",BSS,
".align",ALIGN,
".word",WGEN,
".long",LGEN,
".set",SET,
".end",END,
"rsb",T(JBR,RSB),
"jsb",JSB,
"mfpr",T(MFPR,LONG),
"mtpr",T(MTPR,LONG),
"adawi",0,
"addp4",0,
"addp6",0,
"adwc",0,
"ashp",0,
"bicpsw",0,
"bispsw",0,
"bpt",0,
"bsbb",0,
"bsbw",0,
"chme",0,
"chmk",0,
"chms",0,
"chmu",0,
"cmpc3",0,
"cmpc5",0,
"cmpp3",0,
"cmpp4",0,
"cmpv",0,
"cmpzv",0,
"crc",0,
"cvtlp",0,
"cvtpl",0,
"cvttp",0,
"cvtpt",0,
"cvtps",0,
"cvtsp",0,
"divp",0,
"editpc",0,
"ediv",0,
"emodd",0,
"emodf",0,
"emul",0,
"ffc",0,
"ffs",0,
"halt",0,
"index",0,
"insque",0,
"ldpctx",0,
"locc",0,
"matchc",0,
"movc5",0,
"movp",0,
"movpsl",0,
"movtc",0,
"movtuc",0,
"mulp",0,
"nop",0,
"polyd",0,
"polyf",0,
"popr",0,
"prober",0,
"probew",0,
"pushr",0,
"rei",0,
"remque",0,
"rotl",0,
"sbwc",0,
"scanc",0,
"skpc",0,
"spanc",0,
"subp4",0,
"subp6",0,
"svpctx",0,
"xfc",0,
"escd",0,
"esce",0,
"escf",0,
	0,	0};