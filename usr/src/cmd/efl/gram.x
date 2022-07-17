# define EOS 1
# define NEWLINE 2
# define LPAR 3
# define RPAR 4
# define LBRACK 5
# define RBRACK 6
# define COMMA 7
# define COLON 8
# define ASGNOP 9
# define REPOP 10
# define OR 11
# define AND 12
# define NOT 13
# define RELOP 14
# define ADDOP 15
# define MULTOP 16
# define POWER 17
# define DOUBLEADDOP 18
# define ARROW 19
# define QUALOP 20
# define NAME 21
# define OPTNAME 22
# define STRUCTNAME 23
# define COMNAME 24
# define CONST 25
# define COMMON 26
# define INTERNAL 27
# define EXTERNAL 28
# define AUTOMATIC 29
# define STATIC 30
# define VALUE 31
# define PROCEDURE 32
# define BLOCKDATA 33
# define OPTION 34
# define INCLUDE 35
# define DEFINE 36
# define END 37
# define INTEGER 38
# define REAL 39
# define COMPLEX 40
# define LOGICAL 41
# define CHARACTER 42
# define STRUCT 43
# define FIELD 44
# define ARRAY 45
# define DIMENSION 46
# define LONG 47
# define SHORT 48
# define INITIAL 49
# define EQUIVALENCE 50
# define IMPLICIT 51
# define DEBUG 52
# define IF 53
# define ELSE 54
# define WHILE 55
# define UNTIL 56
# define REPEAT 57
# define DO 58
# define FOR 59
# define SWITCH 60
# define CASE 61
# define DEFAULT 62
# define GO 63
# define GOTO 64
# define BREAK 65
# define EXIT 66
# define NEXT 67
# define RETURN 68
# define CONTINUE 69
# define CALL 70
# define DOUBLE 71
# define PRECISION 72
# define DOUBLEPRECISION 73
# define SIZEOF 74
# define LENGTHOF 75
# define LETTER 76
# define READ 77
# define WRITE 78
# define READBIN 79
# define WRITEBIN 80
# define TRUE 81
# define FALSE 82
# define ESCAPE 83

# line 100 "gram.in"
#include "defs"


extern int prevl, prevv;
ptr p;
ptr procattrs;
int i,n;
static int imptype;
static int ininit =NO;

#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256
short yyexca[] ={
-1, 0,
	0, 1,
	-2, 174,
-1, 1,
	0, -1,
	-2, 0,
-1, 79,
	9, 151,
	19, 151,
	20, 151,
	-2, 177,
-1, 164,
	19, 151,
	20, 151,
	-2, 178,
-1, 261,
	6, 190,
	-2, 14,
-1, 350,
	14, 0,
	-2, 143,
	};
# define YYNPROD 283
# define YYLAST 1450
short yyact[]={

 226, 246, 407, 430, 411, 123, 408, 336, 139,   3,
 224, 196, 156,  20,   6, 155,  19, 184,  60, 257,
 225, 154,  20,   5, 285,  19,  58, 138, 130, 310,
 197, 151,  45, 221, 181,  30,  31,  33,  28,  29,
  32,  21,  22,  16, 134,  14, 296,  34,  35,  36,
  37,  40,  46,  42,  26,  27,  43,  44,  10,  12,
  11, 190,  71,  65, 120,  76,  63, 194, 195,  20,
 211,  17,  19,  59, 445, 424, 412, 178, 188,  79,
  38,  57,  39, 140, 148, 126, 337, 202,  64, 399,
  72,  70, 108,  77,  86, 169, 166, 171, 177, 210,
 174, 175, 176, 153, 398, 198, 217, 232, 324, 199,
 304, 325, 311, 217, 242, 136, 219,  61, 147, 146,
 355, 208, 119,  67,  73, 446, 198, 241, 223, 212,
 199, 215, 214, 213, 216, 228, 212, 133, 215, 214,
 213, 216,  67, 122, 164, 200, 201,  79, 203, 168,
 244,  72, 255, 131,  20, 132, 163,  19, 165, 222,
 108,  77,  86, 259,  75, 160, 200, 201, 272,  61,
 379, 277, 271, 272, 266, 249, 262,  18, 264, 124,
 258, 147, 146, 143, 162, 393, 279,  54, 281, 267,
 268, 269, 125, 238, 147, 146, 234, 432, 286, 278,
 448, 288, 433, 249, 263, 294, 295, 297, 297,  68,
 302, 239, 261,  57, 290, 291, 237, 159, 459, 428,
 251, 250,  64, 343, 342, 306, 270, 271, 272, 147,
 146, 309, 427, 172, 292, 428, 252, 253, 318, 145,
 320, 328,  20, 170,  20,  19, 321,  19,  20, 147,
 146,  19, 312, 282, 299, 322, 416, 316, 391, 329,
 301, 392, 140, 326, 280,   7, 300, 376, 371, 341,
 377, 347, 348, 349, 350, 351, 352, 353, 354, 334,
 363, 303, 153, 364, 356, 359, 366, 338, 362, 461,
 339, 332, 462, 286, 333, 276, 274, 275, 247, 273,
 270, 271, 272, 273, 270, 271, 272, 372, 375, 254,
 142, 374, 158, 370, 464, 233, 381, 365, 357, 360,
 276, 274, 275, 158, 273, 270, 271, 272, 388, 157,
 144,  47, 389, 422, 390,  49, 380,  51,  52, 395,
 417, 413, 235, 133, 397, 444, 419, 236, 396, 276,
 274, 275, 450, 273, 270, 271, 272, 415, 258, 394,
 414, 378, 429, 368, 276, 274, 275, 409, 273, 270,
 271, 272, 127, 405, 314, 278, 406, 426, 278, 249,
 149, 418, 249, 127, 402, 284, 283,  73, 420, 121,
 248, 421, 218, 209, 206,  20, 330, 205,  19, 331,
 423, 439, 276, 274, 275, 204, 273, 270, 271, 272,
 275, 161, 273, 270, 271, 272, 128, 434, 127, 387,
 438, 436, 386, 437, 385, 384, 382, 441, 346, 409,
 344, 440,  48, 443,  50, 442, 409, 447, 449, 141,
   9, 431, 435, 293, 452, 455, 101, 456, 457, 149,
 369, 413, 458, 298, 410, 453, 207, 460, 106, 187,
 109, 185, 289, 463, 186,  98, 191,  67,  96,  45,
 345, 179,  30,  31,  33,  28,  29,  32,  95, 425,
  94, 265, 173,  87,  34,  35,  36,  37,  40,  46,
  42,  26,  27,  43,  44,  85, 305,  84,  83, 340,
 167, 260, 307,  81, 115, 276, 274, 275,  15, 273,
 270, 271, 272, 245,  66, 183, 182,  38, 180,  39,
 192, 193, 149, 112, 113, 110, 111, 194, 195, 383,
 229, 129, 187,  41, 185, 227,  25, 186,  24, 191,
  67,  23,  45, 327, 179,  30,  31,  33,  28,  29,
  32, 308, 220, 335, 152, 240, 317,  34,  35,  36,
  37,  40,  46,  42,  26,  27,  43,  44, 454,  69,
 150,  62,  55, 451, 276, 274, 275, 137, 273, 270,
 271, 272, 276, 274, 275, 135, 273, 270, 271, 272,
  38,  53,  39, 192, 193,   8, 112, 113, 110, 111,
 194, 195, 149, 276, 274, 275, 287, 273, 270, 271,
 272, 256, 187,  74, 185,  13, 231, 186, 319, 191,
  67, 230,  45,   4, 179,  30,  31,  33,  28,  29,
  32,   2,   1,   0,   0,   0,   0,  34,  35,  36,
  37,  40,  46,  42,  26,  27,  43,  44, 403,   0,
   0,   0,   0,   0, 276, 274, 275,   0, 273, 270,
 271, 272,   0,   0,   0,   0,   0,   0,   0,   0,
  38,   0,  39, 192, 193,   0, 112, 113, 110, 111,
 194, 195,   0, 149, 243,   0,   0,   0,   0,   0,
   0,   0,   0, 187,   0, 185,   0,   0, 186,   0,
 191,  67, 189,  45,   0, 179,  30,  31,  33,  28,
  29,  32,   0,   0,   0,   0,   0,   0,  34,  35,
  36,  37,  40,  46,  42,  26,  27,  43,  44, 400,
   0,   0,   0,   0,   0, 276, 274, 275,   0, 273,
 270, 271, 272,   0,   0,   0,   0,   0,   0,   0,
   0,  38,   0,  39, 192, 193, 149, 112, 113, 110,
 111, 194, 195,   0,   0,   0, 187,   0, 185,   0,
   0, 186,   0, 191,  67, 189,  45,   0, 179,  30,
  31,  33,  28,  29,  32,   0,   0,   0,   0,   0,
   0,  34,  35,  36,  37,  40,  46,  42,  26,  27,
  43,  44, 373,   0,   0,   0,   0,   0, 276, 274,
 275,   0, 273, 270, 271, 272,   0,   0,   0,   0,
   0,   0,   0,   0,  38,   0,  39, 192, 193,   0,
 112, 113, 110, 111, 194, 195, 149,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 187,   0, 185,   0,
   0, 186,   0, 191,  67, 189,  45,   0, 179,  30,
  31,  33,  28,  29,  32,   0,   0,   0,   0,   0,
   0,  34,  35,  36,  37, 361,  46,  42,  26,  27,
  43,  44, 367,   0,   0,   0,   0,   0, 276, 274,
 275,   0, 273, 270, 271, 272,   0,   0,   0,   0,
   0,   0,   0,   0,  38,   0,  39, 192, 193,   0,
 112, 113, 110, 111, 194, 195, 315, 149, 276, 274,
 275,   0, 273, 270, 271, 272,   0, 187,   0, 185,
   0,   0, 186,   0, 191,  67, 189,  45,   0, 179,
  30,  31,  33,  28,  29,  32,   0,   0,   0,   0,
   0,   0,  34,  35,  36,  37, 358,  46,  42,  26,
  27,  43,  44, 313,   0,   0,   0,   0,   0, 276,
 274, 275,   0, 273, 270, 271, 272,   0,   0,   0,
   0,   0,   0,   0,   0,  38,   0,  39, 192, 193,
   0, 112, 113, 110, 111, 194, 195,  45,   0,   0,
  30,  31,  33,  28,  29,  32,   0,   0,   0, 189,
  14,   0,  34,  35,  36,  37,  40,  46,  42,  26,
  27,  43,  44,  10,  12,  11,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,  82,  56,
   0,   0,   0,   0,   0,  38,   0,  39,   0,   0,
   0,  78,   0,   0,  90,  61,   0,  45,  91,   0,
  30,  31,  33,  28,  29,  32,  21,  22,   0,   0,
   0,   0,  34,  35,  36,  37,  40,  46,  42,  26,
  27,  43,  44,   0,   0,  89, 103,   0, 105, 189,
 104, 115, 114, 107,  93,  92, 100,  99, 117, 118,
 116,  97, 102,  80,   0,  38,   0,  39,   0,   0,
 112, 113, 110, 111,   0,  61,  88,  45,   0,   0,
  30,  31,  33,  28,  29,  32,   0,   0,   0,   0,
   0,   0,  34,  35,  36,  37,  40,  46,  42,  26,
  27,  43,  44,  10,  12,  11,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,  45,   0,   0,  30,
  31,  33,  28,  29,  32,  38,   0,  39,   0,   0,
 189,  34,  35,  36,  37,  40,  46,  42,  26,  27,
  43,  44,  10,  12,  11,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 149, 404,   0,   0,
   0,   0,   0,   0,  38,   0,  39,   0,   0,   0,
   0,   0,   0,   0,   0,   0,  45,   0,   0,  30,
  31,  33,  28,  29,  32,   0,   0,   0,   0,   0,
 323,  34,  35,  36,  37,  40,  46,  42,  26,  27,
  43,  44,   0,   0,   0, 149, 401,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,  38,  45,  39,   0,  30,  31,
  33,  28,  29,  32,   0,   0,   0,   0,   0,   0,
  34,  35,  36,  37,  40,  46,  42,  26,  27,  43,
  44,   0,   0,   0,   0,   0,  56,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,  61,  38,  45,  39,   0,  30,  31,  33,
  28,  29,  32,   0,   0,   0,   0,   0,   0,  34,
  35,  36,  37,  40,  46,  42,  26,  27,  43,  44,
   0,   0,   0, 149,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,  38,  45,  39,   0,  30,  31,  33,  28,
  29,  32,   0,   0,   0,   0,   0,   0,  34,  35,
  36,  37,  40,  46,  42,  26,  27,  43,  44,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,  45,
   0,   0,  30,  31,  33,  28,  29,  32,   0,   0,
   0,  38,   0,  39,  34,  35,  36,  37,  40,  46,
  42,  26,  27,  43,  44,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,  38,   0,  39 };
short yypact[]={

   9,-1000, 431, 431, 433, 431, 431,-1000,-1000,1034,
 102,-1000, 121, 143,-1000,1033,-1000,-1000,-1000, 386,
 386,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
 176,-1000,-1000,-1000,-1000,-1000,-1000,-1000,  13,-1000,
 415,-1000, 413,-1000,-1000,-1000, 132,-1000,-1000,-1000,
-1000,-1000,-1000,  93,-1000,-1000,1094,-1000, 303,-1000,
 386,-1000, 323,-1000, 230,-1000, 377,-1000,1376, 322,
 316,-1000, 210, 102, 408,-1000,-1000, 175, 102,-1000,
 102,-1000,-1000,1033,-1000,-1000,-1000,-1000,-1000,-1000,
 235,-1000, 225,-1000,-1000,-1000,-1000, 753,-1000,  84,
 127, 402,-1000, 394,-1000, 391,-1000, 390,-1000,  74,
-1000,-1000,-1000,-1000, 389, 753,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000, 753, 519,-1000,
-1000,-1000,-1000,-1000,  70, 308, 187, 341,-1000,-1000,
 303,1291,  96, 184, 102,-1000, 106,  91,-1000, 680,
 291,-1000, 387,1376,-1000,-1000, 386, 384, 102, 102,
 305, 148, 753,  99,-1000,  99,-1000,-1000,-1000,-1000,
-1000, 196,-1000, 753,-1000,-1000,-1000, 593, 162,-1000,
-1000,1340,-1000,-1000,-1000, 753, 102, 753,-1000,-1000,
-1000, -14, 383, 382,-1000,-1000,-1000, 599,-1000,-1000,
 753,-1000, 105,  84, 753, 753, 753, 753, 259, 753,
  81,  85,-1000,-1000,-1000,-1000,-1000,-1000,-1000, 495,
 519,  88,  88, 959, 370,-1000, 908,-1000, 338,1133,
-1000, 974,-1000,-1000,  86,1094,-1000,-1000,-1000,-1000,
 753, 377,-1000,-1000, 392, 287,-1000,1376,  10,-1000,
-1000,-1000,  99,  99,-1000,-1000, 283,-1000,-1000, 593,
-1000,-1000,-1000,-1000,-1000, 216, 593, 429, 427,-1000,
 753, 753, 753, 753, 753, 753, 753, 753,-1000, 151,
  99, 289, 100, 914, 833,-1000, 593, 753, 593, 276,
-1000,-1000, 599, 279, 593, 878, 359, 593,-1000, 261,
-1000,-1000, 798,-1000,-1000,-1000,-1000, 753, 263,-1000,
 357,-1000, 154,-1000,-1000, 519,-1000, 425,-1000,-1000,
 424, 423, 421, 418,-1000,-1000,-1000, 753, 593,-1000,
-1000, 753,-1000, 753,-1000, 254,-1000, 170,-1000,  96,
 333,-1000,-1000, 753,  50,-1000,  33, 156, 151, 151,
 211, 398, 289, 593, 593,-1000, 725,1242, 380, 644,
1193, 369, 593,-1000, 105,-1000, 446,-1000,-1000,-1000,
 356,-1000, 249,-1000,-1000, 339,-1000, 519,-1000,-1000,
-1000, 593,1133, 327,-1000,-1000,-1000,-1000, 593, 593,
 593,-1000,  10,  -1,-1000,-1000,-1000, 593,-1000, 374,
-1000,-1000,-1000,-1000,-1000,-1000,-1000, 228,-1000, 354,
-1000, 194, 336,-1000,-1000,-1000,-1000,-1000,-1000, 753,
-1000,-1000, 400,-1000,-1000,-1000, 753,-1000, 446,-1000,
-1000, 342,  49,-1000, 192, 446, 348,-1000, 572,-1000,
-1000, 564,-1000,-1000, 753,-1000,-1000,-1000,-1000, 212,
-1000,-1000,-1000,-1000,-1000, 285,-1000,-1000,-1000,-1000,
-1000,-1000, 753, 310,-1000 };
short yypgo[]={

   0, 632, 631, 331,   8, 623,  44, 621,  23,  14,
 618, 616, 615, 613, 611, 177, 439,  19,  18, 595,
 591, 585,   5, 577,  27,  26, 572, 571, 197, 570,
   1, 569,  91, 556,  66,  77, 555,   0,  31, 554,
 553,  34,   7,  62,  71,  15, 122,  12,  64, 552,
 551,  10,  20,  73, 543,  21, 541, 538, 536,  33,
  29, 533, 531,  28, 530, 529, 518,  17, 516, 515,
  78,  63, 514, 513,  61, 508,  65, 503, 501, 500,
 499, 498, 497, 495, 483, 482, 481, 480, 479, 478,
 470, 468, 465,  11,  30,  24, 462, 460,  70,  46,
 458, 456, 453, 450,  76, 446, 443,   2,   6,   3,
   4, 442, 441 };
short yyr1[]={

   0,   1,   1,   1,   1,   1,   1,   1,   3,  10,
  10,  10,  10,   6,  11,   6,   5,   5,   5,   5,
  13,  12,  12,  15,  15,  14,  14,  17,   2,  19,
  20,  20,  20,  21,  21,  21,   8,   7,  22,  23,
  23,  23,  24,  24,   4,   4,   4,   4,   4,   4,
  33,  33,  33,  27,  27,  36,  34,  29,  29,  38,
  38,  39,  40,  40,  42,  42,  31,  31,  43,  32,
  32,  16,  16,  44,  44,  46,  46,  49,  48,  50,
  50,  51,  51,  52,  52,  26,  26,  25,  25,  53,
  54,  53,  18,  41,  41,  55,  55,  45,  45,  45,
  56,  56,  56,  56,  56,  56,  56,  59,  60,  60,
  57,  57,  57,  57,  57,  57,  57,  57,  57,  61,
  61,  61,  62,  62,  64,  65,  63,  47,  47,  58,
  58,  37,  37,  37,  37,  37,  37,  37,  37,  37,
  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,
  37,  35,  71,  71,  71,  71,  71,  72,  67,  67,
  67,  73,  73,  68,  68,  68,  69,  69,  69,  66,
  66,  74,  74,   9,  75,  76,  76,  76,  76,  76,
  76,  76,  76,  76,  76,  76,  76,  84,  84,  79,
  80,  81,  81,  81,  81,  85,  81,  86,  86,  82,
  82,  82,  82,  87,  88,  89,  90,  90,  83,  83,
  83,  83,  83,  83,  83,  94,  96,  96,  95,  95,
  93,  93,  93,  93,  92,  92,  92,  92,  92,  97,
  97,  97,  98,  98,  98,  98,  98,  98,  91,  91,
  91,  91, 104, 104, 104, 104, 104, 104, 100, 101,
 101, 102, 103, 103,  99,  99,  77,  78,  70, 105,
 105, 105, 105, 106, 106, 107, 107, 108, 108, 108,
 108, 108, 108, 108, 111, 110, 109, 109, 109, 109,
 112,  28,  30 };
short yyr2[]={

   0,   0,   2,   2,   4,   2,   2,   1,   1,   2,
   2,   2,   2,   0,   0,   3,   1,   2,   4,   5,
   1,   1,   2,   1,   1,   1,   3,   1,   2,   1,
   0,   2,   3,   1,   3,   3,   1,   1,   0,   1,
   2,   3,   1,   1,   2,   4,   2,   4,   2,   2,
   1,   2,   3,   1,   3,   0,   4,   1,   3,   1,
   4,   1,   1,   3,   1,   3,   1,   3,   3,   3,
   3,   1,   2,   2,   2,   0,   1,   0,   4,   1,
   3,   1,   3,   1,   1,   0,   1,   1,   3,   2,
   0,   5,   1,   1,   2,   1,   2,   2,   1,   1,
   1,   1,   5,   5,   1,   1,   1,   0,   0,   1,
   1,   1,   1,   1,   2,   1,   4,   1,   4,   1,
   4,   2,   1,   1,   0,   0,   6,   1,   1,   1,
   1,   1,   1,   1,   2,   1,   1,   1,   3,   3,
   3,   2,   2,   3,   3,   3,   2,   3,   3,   1,
   1,   1,   1,   2,   3,   4,   3,   1,   2,   3,
   3,   3,   3,   4,   4,   4,   4,   4,   4,   1,
   3,   1,   1,   2,   0,   3,   2,   1,   2,   3,
   5,   2,   1,   1,   1,   1,   1,   0,   1,   0,
   0,   3,   2,   4,   3,   0,   5,   1,   3,   8,
   4,   4,   3,   4,   0,   1,   1,   5,   1,   2,
   1,   2,   3,   3,   4,   3,   1,   3,   1,   2,
   1,   1,   2,   1,   1,   2,   2,   3,   3,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   4,   4,
   4,   1,   4,   3,   6,   5,   8,   7,   2,   2,
   4,   2,   2,   4,   0,   1,   2,   0,   6,   1,
   1,   1,   1,   1,   0,   1,   3,   1,   3,   2,
   1,   2,   4,   3,   0,   4,   1,   4,   6,   3,
   3,   0,   0 };
short yychk[]={

-1000,  -1,  -2,  -4,  -5,  -8,  -9, 256, -19, -16,
  49,  51,  50, -12,  36, -75,  34, -44, -15, -45,
 -47,  32,  33, -56, -57, -58,  45,  46,  29,  30,
  26,  27,  31,  28,  38,  39,  40,  41,  71,  73,
  42, -61,  44,  47,  48,  23,  43,  -3,   1,  -3,
   1,  -3,  -3, -20, -15, -26,   5, -44, -25, -53,
 -18,  21, -27, -34, -35, -71, -72,  21, -28, -31,
 -32, -43, -35,   3, -13,  21, -76, -35,  18, -71,
  70, -77,   5, -81, -82, -83, -70, -84,  83,  52,
  21,  25,  62,  61, -87, -89, -91,  68, -92,  64,
  63,-105,  69,  53,  57,  55,-100,  60,-104, -97,
  79,  80,  77,  78,  59,  58,  67,  65,  66, -46,
 -48,   3, -46, -22,   3,  16,  72,   3,   3, -62,
 -63,  21,  23,   5,  -6, -21,  22, -23, -24,  -4,
 -25, -16,   7, -46,   7,   9,  20,  19, -67,   3,
 -29, -38, -39, -41, -55, -45, -47,   7,   7,   7,
 -32,   3,   9, -35, -71, -35,  -9, -79, -76, -22,
   8, -22,   8, -85, -22, -22, -22, -37, -35,  25,
 -66, -41, -68, -69, -67,  15,  18,  13, -70, 256,
 -74,  20,  74,  75,  81,  82, -93, -94,  21,  25,
  61,  62,   3,  21,   3,   3,   3,-101,  -9,   3,
  25, -98,  55,  59,  58,  57,  60,  32,   3, -37,
 -49, -59, -59, -37, -51, -52, -37,  16, -22, -64,
  -7, -11,  37,   7,   9,   1,   6, -53,   9, -34,
 -36,  21,  23,   4, -37, -73, -30,   7,   3, -55,
 -48, -43, -35, -35,   4,   4, -14, -17, -18, -37,
 -78,  -6, -22,   8, -22, -86, -37,  -9,  -9,  -9,
  15,  16,  17,  14,  11,  12,  10,   9, -67, -37,
 -35, -37, -74,   3,   3, -95, -37,   7, -37, -96,
 -93, -93, -94,-106, -37, -37, -99, -37,-102, -99,
   7,   1, -37, -98,  25,   1, -22,   7, -50, -51,
 -60,  24, -60,   4,   4,   8, -63, -33,  -4, -10,
  -4,  -9,  -8, 256,  22,  25, -24, -54, -37, -67,
   4,   7,   4,   7, -38, -40, -42,  76,   4,   7,
 -80, -22,   8,   7,   1, -90,   1, -37, -37, -37,
 -37, -37, -37, -37, -37,  20, -37, -41,  42, -37,
 -41,  42, -37,   4,   7, -95,   7,   4,   4,-103,
  -9,   7, -22,   4, -22, -37,   4,   7,   4,  16,
 -52, -37,   1, -65,   1,   1,   1,   1, -37, -37,
 -37,   4,   7,  15, -17,   6, -22, -37,  54,  56,
   4,   4,   4,   4,   4,   4, -93,-107,-108, -37,
   8,-110,-104,   5,   4,   1,   7,   1, -22,   7,
 -51,  -4,   6, -42,  76, -88,   3,   4,   7,   8,
-109,-112, -28,   8,-110,-111, -22, -22, -37,   1,
 -22, -37,-108,-109,   3,  25,  76,-109,   8,-107,
   4,   1, -22,  -9,   4, -37, -30, -30,-109,   6,
 -22,   4,   7, -37,   4 };
short yydef[]={

  -2,  -2,   0,   0,   0,   0,   0,   7,  30,  85,
   0, 281,   0,  16,  36, 187,  29,  71,  21,  75,
  75,  23,  24,  38,  98,  99, 127, 128, 100, 101,
   0, 104, 105, 106, 110, 111, 112, 113,   0, 115,
   0, 117,   0, 129, 130, 119,   0,   2,   8,   3,
  13,   5,   6,  28,  22,  44,   0,  72,  86,  87,
  75,  92,  46,  53,   0, 151, 152, 157,   0,  48,
  49,  66,   0,   0,  17,  20, 173,   0,   0,  -2,
   0, 174, 189, 187, 182, 183, 184, 185, 186,  38,
 157,  38,   0, 195,  38,  38,  38, 208, 210,   0,
   0,   0, 188,   0, 205,   0, 174,   0, 241, 224,
 259, 260, 261, 262,   0,   0, 229, 230, 231,  73,
  76,  77,  74,  97, 107, 107, 114,   0,   0,  38,
 121, 122, 123, 124,  14,  31,  33,   0,  39,  42,
  43,  85,   0,  89,   0,  55,   0,   0, 153,   0,
 282,  57,  59,  61,  93,  95,   0,   0,   0,   0,
   0,   0,   0, 176,  -2,   0, 257,  13, 181, 256,
  38, 192,  38,   0, 174, 174, 174, 209, 131, 132,
 133,   0, 135, 136, 137,   0,   0,   0, 149, 150,
 169,   0,   0,   0, 171, 172, 211,   0, 220, 221,
   0, 223,   0,   0, 264,   0, 254, 254,   0,   0,
 225, 226, 232, 233, 234, 235, 236, 237, 248,  38,
   0, 108, 108,   0,   0,  81,  83,  84,   0,   0,
   4, 174,  37,  32,   0,  40,  45,  88,  90,  54,
   0, 154, 156, 158,   0,   0,  47,   0,   0,  94,
  96,  67,  70,  69,  68,  18,   0,  25,  27, 175,
 179,  -2, 191,  38, 194,   0, 197,   0,   0, 202,
   0,   0,   0,   0,   0,   0,   0,   0, 134, 141,
 142, 146,   0,   0,   0, 213, 218,   0, 222,   0,
 216, 212,   0,   0, 263,   0,   0, 255, 174,   0,
 249,  38,   0, 227, 228,  38, 243,   0,   0,  79,
   0, 109,   0, 116, 118,   0, 120, 125,  50,  15,
   0,   0,   0,   0,  34,  35,  41,   0,  56, 155,
 159,   0, 160,   0,  58,   0,  62,  64,  19,   0,
   0, 193,  38,   0, 200, 201, 206, 138, 139, 140,
  -2, 144, 145, 148, 147, 170,   0,   0,   0,   0,
   0,   0, 219, 215,   0, 214,   0, 203, 238, 239,
   0, 251,   0, 240, 242,  38,  78,   0, 102, 103,
  82,  83,  51,   0,   9,  10,  11,  12,  91, 161,
 162,  60,   0,   0,  26, 180, 196, 198, 204,   0,
 163, 164, 165, 166, 167, 168, 217,   0, 265, 267,
 281, 270,   0, 274, 252,  38, 250,  38, 245,   0,
  80,  52,   0,  63,  65,  38,   0, 258,   0, 281,
 269, 276,   0, 281, 271,   0,   0, 244,  38, 126,
 174,   0, 266, 268,   0, 282, 282, 273, 281,   0,
 253,  38, 247, 199, 207,   0, 279, 280, 272, 275,
 246, 277,   0,   0, 278 };
#
# define YYFLAG -1000
# define YYERROR goto yyerrlab
# define YYACCEPT return(0)
# define YYABORT return(1)

/*	parser for yacc output	*/

int yydebug = 0; /* 1 for debugging */
YYSTYPE yyv[YYMAXDEPTH]; /* where the values are stored */
int yychar = -1; /* current input token number */
int yynerrs = 0;  /* number of errors */
short yyerrflag = 0;  /* error recovery flag */

yyparse() {

	short yys[YYMAXDEPTH];
	short yyj, yym;
	register YYSTYPE *yypvt;
	register short yystate, *yyps, yyn;
	register YYSTYPE *yypv;
	register short *yyxi;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyps= &yys[-1];
	yypv= &yyv[-1];

 yystack:    /* put a state and value onto the stack */

	if( yydebug  ) printf( "state %d, char 0%o\n", yystate, yychar );
		if( ++yyps> &yys[YYMAXDEPTH] ) { yyerror( "yacc stack overflow" ); return(1); }
		*yyps = yystate;
		++yypv;
		*yypv = yyval;

 yynewstate:

	yyn = yypact[yystate];

	if( yyn<= YYFLAG ) goto yydefault; /* simple state */

	if( yychar<0 ) if( (yychar=yylex())<0 ) yychar=0;
	if( (yyn += yychar)<0 || yyn >= YYLAST ) goto yydefault;

	if( yychk[ yyn=yyact[ yyn ] ] == yychar ){ /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if( yyerrflag > 0 ) --yyerrflag;
		goto yystack;
		}

 yydefault:
	/* default state action */

	if( (yyn=yydef[yystate]) == -2 ) {
		if( yychar<0 ) if( (yychar=yylex())<0 ) yychar = 0;
		/* look through exception table */

		for( yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate) ; yyxi += 2 ) ; /* VOID */

		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
			}
		if( (yyn = yyxi[1]) < 0 ) return(0);   /* accept */
		}

	if( yyn == 0 ){ /* error */
		/* error ... attempt to resume parsing */

		switch( yyerrflag ){

		case 0:   /* brand new error */

			yyerror( "syntax error" );
		yyerrlab:
			++yynerrs;

		case 1:
		case 2: /* incompletely recovered error ... try again */

			yyerrflag = 3;

			/* find a state where "error" is a legal shift action */

			while ( yyps >= yys ) {
			   yyn = yypact[*yyps] + YYERRCODE;
			   if( yyn>= 0 && yyn < YYLAST && yychk[yyact[yyn]] == YYERRCODE ){
			      yystate = yyact[yyn];  /* simulate a shift of "error" */
			      goto yystack;
			      }
			   yyn = yypact[*yyps];

			   /* the current yyps has no shift onn "error", pop stack */

			   if( yydebug ) printf( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
			   --yyps;
			   --yypv;
			   }

			/* there is no state on the stack with an error shift ... abort */

	yyabort:
			return(1);


		case 3:  /* no shift yet; clobber input char */

			if( yydebug ) printf( "error recovery discards char %d\n", yychar );

			if( yychar == 0 ) goto yyabort; /* don't discard EOF, quit */
			yychar = -1;
			goto yynewstate;   /* try again in the same state */

			}

		}

	/* reduction by production yyn */

		if( yydebug ) printf("reduce %d\n",yyn);
		yyps -= yyr2[yyn];
		yypvt = yypv;
		yypv -= yyr2[yyn];
		yyval = yypv[1];
		yym=yyn;
			/* consult goto table to find next state */
		yyn = yyr1[yyn];
		yyj = yypgo[yyn] + *yyps + 1;
		if( yyj>=YYLAST || yychk[ yystate = yyact[yyj] ] != -yyn ) yystate = yyact[yypgo[yyn]];
		switch(yym){
			
case 1:
# line 116 "gram.in"
{ graal = PARSEOF; } break;
case 2:
# line 118 "gram.in"
{ graal = PARSOPT; } break;
case 3:
# line 120 "gram.in"
{ graal = PARSDCL; doinits(yypvt[-1]);  frchain( & yypvt[-1]); } break;
case 4:
# line 122 "gram.in"
{ endproc(); graal = PARSPROC; } break;
case 5:
# line 124 "gram.in"
{ graal = PARSDEF; } break;
case 6:
# line 126 "gram.in"
{ graal = PARSERR; } break;
case 7:
# line 128 "gram.in"
{ graal = PARSERR;
		  errmess("Syntax error", "", "");
		} break;
case 8:
# line 133 "gram.in"
{ eofneed = 1; } break;
case 9:
# line 136 "gram.in"
{ if(!dclsect)
			warn("declaration amid executables");
		    yyval = bgnexec();
		   TEST fprintf(diagfile,"stat: dcl\n");
		  doinits(yypvt[-1]); frchain( & yypvt[-1]); } break;
case 10:
# line 142 "gram.in"
{ if(dclsect && yypvt[-1]->tag!=TSTFUNCT)
			dclsect = 0;
		    TEST fprintf(diagfile, "stat: exec\n"); } break;
case 11:
# line 146 "gram.in"
{ yyval = bgnexec(); } break;
case 12:
# line 148 "gram.in"
{ yyerrok;
		  errmess("Syntax error", "", "");
		  yyval = bgnexec();
		} break;
case 13:
# line 155 "gram.in"
{ yyval = bgnexec(); } break;
case 14:
# line 156 "gram.in"
{ thisexec->copylab = 1; } break;
case 15:
# line 157 "gram.in"
{ yyval = addexec(); thisexec->copylab = 0; } break;
case 16:
# line 161 "gram.in"
{ procname = 0; thisargs = 0;
		  if(procclass == 0) procclass = PRMAIN;
		  goto proctype;
		} break;
case 17:
# line 166 "gram.in"
{ thisargs = 0; goto proctype; } break;
case 18:
# line 168 "gram.in"
{ thisargs = 0; goto proctype; } break;
case 19:
# line 170 "gram.in"
{ thisargs = yypvt[-1];
	proctype:
		if(procattrs)
			if(procname == 0)
				dclerr("attributes on unnamed procedure", "");
			else	{
				attvars(procattrs, mkchain(procname,0));
				procclass = PRFUNCT;
				}
		fprintf(diagfile, "Procedure %s:\n", procnm() );
		if(verbose)
			fprintf(diagfile, "    Pass 1\n");
		} break;
case 20:
# line 186 "gram.in"
{ procname = mkvar(yypvt[-0]);
		  extname(procname);
		} break;
case 21:
# line 192 "gram.in"
{ procattrs = 0; } break;
case 22:
# line 194 "gram.in"
{ procattrs = yypvt[-1];
		  if(procclass == 0) procclass = PRFUNCT;
		} break;
case 23:
# line 200 "gram.in"
{ yyval = bgnproc(); procclass = 0; } break;
case 24:
# line 202 "gram.in"
{ yyval = bgnproc(); procclass = PRBLOCK; } break;
case 25:
# line 206 "gram.in"
{ yyval = mkchain(yypvt[-0],0); } break;
case 26:
# line 208 "gram.in"
{ hookup(yypvt[-2], mkchain(yypvt[-0],0) ); } break;
case 27:
# line 212 "gram.in"
{ if(yypvt[-0]->vclass == CLUNDEFINED)
			yypvt[-0]->vclass = CLARG;
		  else dclerr("argument already used", yypvt[-0]->sthead->namep);
		} break;
case 28:
# line 218 "gram.in"
{ optneed = 0; } break;
case 29:
# line 222 "gram.in"
{ if(blklevel > 0)
			{
			execerr("Option statement inside procedure", "");
			execerr("procedure %s terminated prematurely", procnm());
			endproc();
			}
		  optneed = 1;
		  } break;
case 33:
# line 238 "gram.in"
{ setopt(yypvt[-0],0); cfree(yypvt[-0]); } break;
case 34:
# line 240 "gram.in"
{ setopt(yypvt[-2],yypvt[-0]); cfree(yypvt[-2]); cfree(yypvt[-0]); } break;
case 35:
# line 242 "gram.in"
{ setopt(yypvt[-2],yypvt[-0]->leftp); cfree(yypvt[-2]); cfree(yypvt[-0]); } break;
case 36:
# line 246 "gram.in"
{ defneed = 1; } break;
case 37:
# line 250 "gram.in"
{ if(thisctl->subtype != STPROC)
			execerr("control stack not empty upon END", "");
		  exnull();
		  popctl();
		} break;
case 38:
# line 258 "gram.in"
{ igeol=1; /* continue past newlines  */ } break;
case 41:
# line 263 "gram.in"
{ yyval = hookup(yypvt[-2],yypvt[-0]); } break;
case 44:
# line 271 "gram.in"
{ attvars(yypvt[-1],yypvt[-0]); yyval = yypvt[-0]; } break;
case 45:
# line 273 "gram.in"
{ attvars(yypvt[-3],yypvt[-1]); yyval = yypvt[-1]; } break;
case 46:
# line 275 "gram.in"
{ yyval = 0; } break;
case 47:
# line 277 "gram.in"
{ yyval = 0; } break;
case 48:
# line 279 "gram.in"
{ yyval = 0; } break;
case 49:
# line 281 "gram.in"
{ mkequiv(yypvt[-0]); yyval = 0; } break;
case 52:
# line 287 "gram.in"
{ yyval = hookup(yypvt[-2],yypvt[-0]); } break;
case 55:
# line 294 "gram.in"
{ininit = YES; } break;
case 56:
# line 295 "gram.in"
 { ininit = NO;  mkinit(yypvt[-3],yypvt[-0]);  frexpr(yypvt[-3]); } break;
case 59:
# line 303 "gram.in"
{ setimpl(imptype, 'a', 'z'); } break;
case 61:
# line 308 "gram.in"
{ imptype = yypvt[-0]->attype; cfree(yypvt[-0]); } break;
case 64:
# line 316 "gram.in"
{ setimpl(imptype, yypvt[-0], yypvt[-0]); } break;
case 65:
# line 318 "gram.in"
{ setimpl(imptype, yypvt[-2], yypvt[-0]); } break;
case 68:
# line 326 "gram.in"
{ mkequiv(yypvt[-1]); } break;
case 69:
# line 330 "gram.in"
{ yyval = mkchain(yypvt[-2], mkchain(yypvt[-0],0)); } break;
case 70:
# line 332 "gram.in"
{ yyval = hookup(yypvt[-2], mkchain(yypvt[-0],0)); } break;
case 72:
# line 336 "gram.in"
{ attatt(yypvt[-1],yypvt[-0]); } break;
case 73:
# line 339 "gram.in"
{ yypvt[-1]->atdim = yypvt[-0]; } break;
case 74:
# line 340 "gram.in"
{ yyval = ALLOC(atblock); yyval->atdim = yypvt[-0]; } break;
case 75:
# line 343 "gram.in"
{ yyval = 0; } break;
case 77:
# line 347 "gram.in"
{ inbound = 1; } break;
case 78:
# line 348 "gram.in"
{ inbound = 0;  yyval = arrays = mkchain(yypvt[-1],arrays); } break;
case 80:
# line 352 "gram.in"
{ hookup(yypvt[-2],yypvt[-0]); } break;
case 81:
# line 356 "gram.in"
{
		yyval = ALLOC(dimblock);
		yyval->lowerb = 0;
		yyval->upperb = yypvt[-0];
		} break;
case 82:
# line 362 "gram.in"
{
		yyval = ALLOC(dimblock);
		yyval->lowerb = yypvt[-2];
		yyval->upperb = yypvt[-0];
		} break;
case 84:
# line 370 "gram.in"
{ yyval = 0; } break;
case 85:
# line 373 "gram.in"
{ yyval = 0; } break;
case 88:
# line 378 "gram.in"
{ hookup(yypvt[-2],yypvt[-0]); } break;
case 89:
# line 382 "gram.in"
{
		if(yypvt[-0]!=0)
			if(yypvt[-1]->vdim==0)
				yypvt[-1]->vdim = yypvt[-0];
			else if(!eqdim(yypvt[-0],yypvt[-1]->vdim))
				dclerr("multiple dimension", yypvt[-1]->namep);
		yyval = mkchain(yypvt[-1],0);
		} break;
case 90:
# line 390 "gram.in"
{ ininit = YES; } break;
case 91:
# line 391 "gram.in"
{
		ininit = NO;
		if(yypvt[-2]!=OPASGN)
			dclerr("illegal initialization operator", yypvt[-4]->sthead->namep);
		if(yypvt[-3]!=0)
			if(yypvt[-4]->vdim==0)
				yypvt[-4]->vdim = yypvt[-3];
			else if(!eqdim(yypvt[-3],yypvt[-4]->vdim))
				dclerr("multiple dimension", yypvt[-4]->sthead->namep);
		if(yypvt[-0]!=0 && yypvt[-4]->vinit!=0)
			dclerr("multiple initialization", yypvt[-4]->sthead->namep);
		yypvt[-4]->vinit = yypvt[-0];
		yyval = mkchain(yypvt[-4],0);
		} break;
case 92:
# line 408 "gram.in"
{ yyval = mkvar(yypvt[-0]); } break;
case 94:
# line 413 "gram.in"
{ attatt(yypvt[-1],yypvt[-0]); } break;
case 96:
# line 418 "gram.in"
{ yyval = ALLOC(atblock); yyval->atdim = yypvt[-0]; } break;
case 97:
# line 422 "gram.in"
{
		yyval = ALLOC(atblock);
		if(yypvt[-1] == CLEXT)
			yyval->atext = 1;
		else if(yypvt[-1]>=0 && yypvt[-1]<=10)
			yyval->atclass = yypvt[-1];
		else	{
			yyval->atclass = CLCOMMON;
			yyval->atcommon = yypvt[-1];
			}
		} break;
case 98:
# line 434 "gram.in"
{
		yyval = ALLOC(atblock);
		if(yypvt[-0] >=0 && yypvt[-0]<10)
			yyval->attype = yypvt[-0];
		else	{
			yyval->attypep = yypvt[-0];
			if(yypvt[-0]->tag == TSTRUCT)
				yyval->attype = TYSTRUCT;
			else if(yypvt[-0]->tag == TFIELD)
				yyval->attype = TYFIELD;
			else	yyval->attype = TYCHAR;
			}
		} break;
case 99:
# line 448 "gram.in"
{
		yyval = ALLOC(atblock);
		yyval->atprec = yypvt[-0];
		} break;
case 100:
# line 454 "gram.in"
{ yyval = CLAUTO; fprintf(diagfile, "AUTOMATIC not yet implemented\n"); } break;
case 101:
# line 455 "gram.in"
{ yyval = CLSTAT; } break;
case 102:
# line 457 "gram.in"
{ yyval = yypvt[-1]; } break;
case 103:
# line 459 "gram.in"
{ yyval = yypvt[-1]; } break;
case 104:
# line 460 "gram.in"
{ yyval = CLSTAT; } break;
case 105:
# line 461 "gram.in"
{ yyval = CLVALUE; fprintf(diagfile, "VALUE not yet implemented\n");  } break;
case 106:
# line 462 "gram.in"
{ yyval = CLEXT; } break;
case 107:
# line 465 "gram.in"
{ comneed = 1; } break;
case 108:
# line 468 "gram.in"
{ yyval = mkcomm(""); } break;
case 110:
# line 472 "gram.in"
{ yyval = TYINT; } break;
case 111:
# line 473 "gram.in"
{ yyval = TYREAL; } break;
case 112:
# line 474 "gram.in"
{ yyval = TYCOMPLEX; } break;
case 113:
# line 475 "gram.in"
{ yyval = TYLOG; } break;
case 114:
# line 477 "gram.in"
{ yyval = TYLREAL; /* holdover from Fortran */ } break;
case 115:
# line 479 "gram.in"
{ yyval = TYLREAL; /* holdover from Fortran */ } break;
case 116:
# line 481 "gram.in"
{ yyval = yypvt[-1]; } break;
case 118:
# line 483 "gram.in"
{ yyval = mkfield(yypvt[-1]); } break;
case 119:
# line 487 "gram.in"
{ yyval = yypvt[-0]->varp; } break;
case 120:
# line 489 "gram.in"
{ yyval = mkstruct(yypvt[-2],yypvt[-0]); } break;
case 121:
# line 491 "gram.in"
{ yyval = mkstruct(0,yypvt[-0]); } break;
case 122:
# line 495 "gram.in"
{ if(yypvt[-0]->varp && yypvt[-0]->varp->blklevel<blklevel)
			hide(yypvt[-0]);
		  yypvt[-0]->tag = TSTRUCT;
		} break;
case 123:
# line 500 "gram.in"
{ if(yypvt[-0]->varp)
			if(yypvt[-0]->varp->blklevel<blklevel)
				hide(yypvt[-0]);
			else dclerr("multiple declaration for type %s", yypvt[-0]->namep);
		} break;
case 124:
# line 507 "gram.in"
{ ++instruct; } break;
case 125:
# line 507 "gram.in"
{ --instruct; } break;
case 126:
# line 508 "gram.in"
{ yyval = yypvt[-3]; prevv = -1; } break;
case 129:
# line 515 "gram.in"
{ yyval = 1; } break;
case 130:
# line 516 "gram.in"
{ yyval = 0; } break;
case 131:
# line 519 "gram.in"
{ if(yypvt[-0]->tag == TCALL)
			yypvt[-0] = funcinv(yypvt[-0]);
		  if(yypvt[-0]->vtype==TYUNDEFINED && yypvt[-0]->vext==0)
			impldecl(yypvt[-0]);
		  else if(yypvt[-0]->tag==TNAME && yypvt[-0]->vdcldone==0
			  && yypvt[-0]->vext==0 && !inbound)
				dclit(yypvt[-0]);
		  if(yypvt[-0]->vtype==TYFIELD)
			yyval = extrfield(yypvt[-0]);
		} break;
case 133:
# line 531 "gram.in"
{ yyval = mkconst(TYLOG, (yypvt[-0] == TRUE ? ".true." : ".false.") ); } break;
case 134:
# line 533 "gram.in"
{ yyval = typexpr(yypvt[-1],yypvt[-0]); } break;
case 137:
# line 537 "gram.in"
{ if( !ininit && yypvt[-0]->tag== TLIST)
			yyval = compconst(yypvt[-0]); 
		  else yypvt[-0]->needpar = 1; } break;
case 138:
# line 541 "gram.in"
{ yyval = mknode(TAROP,yypvt[-1],yypvt[-2],yypvt[-0]); } break;
case 139:
# line 543 "gram.in"
{ yyval = mknode(TAROP,yypvt[-1],yypvt[-2],yypvt[-0]); } break;
case 140:
# line 545 "gram.in"
{ yyval = mknode(TAROP,yypvt[-1],yypvt[-2],yypvt[-0]); } break;
case 141:
# line 547 "gram.in"
{ if(yypvt[-1]==OPMINUS)
			yyval = mknode(TNEGOP,OPMINUS, yypvt[-0],0);
		  else	yyval = yypvt[-0];  } break;
case 142:
# line 551 "gram.in"
{ yyval =  mknode(TASGNOP,yypvt[-1],yypvt[-0],mkint(1)); } break;
case 143:
# line 553 "gram.in"
{ yyval = mknode(TRELOP,yypvt[-1],yypvt[-2],yypvt[-0]); } break;
case 144:
# line 555 "gram.in"
{ yyval = mknode(TLOGOP,yypvt[-1],yypvt[-2],yypvt[-0]); } break;
case 145:
# line 557 "gram.in"
{ yyval = mknode(TLOGOP,yypvt[-1],yypvt[-2],yypvt[-0]); } break;
case 146:
# line 559 "gram.in"
{ yyval = mknode(TNOTOP,yypvt[-1],yypvt[-0],0); } break;
case 147:
# line 561 "gram.in"
{ if(yypvt[-2]->tag == TCALL)
			{
			exprerr("may not assign to a function", NULL);
			yyval = errnode();
			}
		  else
			yyval = mknode(TASGNOP,yypvt[-1],yypvt[-2],yypvt[-0]);
		} break;
case 148:
# line 570 "gram.in"
{ yyval = mknode(TREPOP,0,yypvt[-2],yypvt[-0]); } break;
case 150:
# line 573 "gram.in"
{ yyval = errnode(); } break;
case 151:
# line 577 "gram.in"
{ if(yypvt[-0]->tag==TNAME && yypvt[-0]->vdcldone==0 &&
			yypvt[-0]->vsubs==0 && yypvt[-0]->vext==0 && !inbound)
				dclit(yypvt[-0]);
		} break;
case 153:
# line 585 "gram.in"
{
		if(yypvt[-0]->tag!=TLIST)
			yypvt[-0] = mknode(TLIST,0, mkchain(yypvt[-0],0) ,0);
		if(yypvt[-1]->vdim)
			{
			if(yypvt[-1]->vdcldone==0 && yypvt[-1]->vext==0)
				dclit(yypvt[-1]);
			yyval = subscript(yypvt[-1],yypvt[-0]);
			}
		else	yyval = mkcall(yypvt[-1],yypvt[-0]);
		} break;
case 154:
# line 597 "gram.in"
{ yyval = strucelt(yypvt[-2],yypvt[-0]); } break;
case 155:
# line 599 "gram.in"
{ if(yypvt[-0]->tag != TLIST)
			yypvt[-0] = mknode(TLIST,0, mkchain(yypvt[-0],0),0);
		  yyval = subscript(strucelt(yypvt[-3],yypvt[-1]), yypvt[-0]);
		} break;
case 156:
# line 604 "gram.in"
{ yyval = mkarrow(yypvt[-2],yypvt[-0]); } break;
case 157:
# line 608 "gram.in"
{ if(yypvt[-0]->varp == 0) mkvar(yypvt[-0]);
		  if(inbound)
			  yypvt[-0]->varp->vadjdim = 1;
		  yyval = cpexpr(yypvt[-0]->varp); } break;
case 158:
# line 615 "gram.in"
{ yyval = mknode(TLIST,0, 0,0); } break;
case 159:
# line 617 "gram.in"
{ yyval = yypvt[-1]; } break;
case 160:
# line 619 "gram.in"
{ yyval = mknode(TLIST,0,yypvt[-1],0); } break;
case 161:
# line 623 "gram.in"
{ yyval = mkchain(yypvt[-2], mkchain(yypvt[-0],0) ); } break;
case 162:
# line 625 "gram.in"
{ hookup(yypvt[-2], mkchain(yypvt[-0],0) ); } break;
case 163:
# line 629 "gram.in"
{ yyval = esizeof(yypvt[-1]->vtype, yypvt[-1]->vtypep, yypvt[-1]->vdim);
		  frexpr(yypvt[-1]); } break;
case 164:
# line 632 "gram.in"
{ if(yypvt[-1]->attype==TYREAL && yypvt[-1]->atprec)
			yypvt[-1]->attype = TYLREAL;
		  yyval = esizeof(yypvt[-1]->attype, yypvt[-1]->attypep, yypvt[-1]->atdim);
		  cfree(yypvt[-1]);
		} break;
case 165:
# line 638 "gram.in"
{ yyval = mkint(tailor.ftnsize[FTNINT]/tailor.ftnchwd); } break;
case 166:
# line 642 "gram.in"
{ yyval = elenof(yypvt[-1]->vtype, yypvt[-1]->vtypep, yypvt[-1]->vdim);
		  frexpr(yypvt[-1]); } break;
case 167:
# line 645 "gram.in"
{ yyval = elenof(yypvt[-1]->attype, yypvt[-1]->attypep, yypvt[-1]->atdim);
		  cfree(yypvt[-1]);
		} break;
case 168:
# line 649 "gram.in"
{ yyval = mkint(1); } break;
case 170:
# line 654 "gram.in"
{ yyval = yypvt[-1]; } break;
case 173:
# line 661 "gram.in"
{ TEST fprintf(diagfile, "exec done\n"); } break;
case 174:
# line 665 "gram.in"
{ yyval = bgnexec();  if(ncases > 0) ncases = 0; } break;
case 175:
# line 669 "gram.in"
{
		if(yypvt[-2]->tag==TCALL)
			{
			dclerr("no statement functions in EFL",
				yypvt[-2]->sthead->namep);
			frexpr(yypvt[-2]);
			frexpr(yypvt[-0]);
			}
		else exasgn(yypvt[-2],yypvt[-1],yypvt[-0]);
		} break;
case 176:
# line 680 "gram.in"
{ exasgn(yypvt[-0], yypvt[-1], mkint(1) ); } break;
case 177:
# line 682 "gram.in"
{ excall(yypvt[-0]); } break;
case 178:
# line 684 "gram.in"
{ excall(yypvt[-0]); } break;
case 180:
# line 687 "gram.in"
{ TEST fprintf(diagfile, "exec: { stats }\n");
		  addexec(); } break;
case 181:
# line 690 "gram.in"
{ thisexec->labeled = 1; } break;
case 182:
# line 692 "gram.in"
{ thisexec->uniffable = 1;  popctl(); } break;
case 183:
# line 694 "gram.in"
{ thisexec->brnchend = 1; } break;
case 184:
# line 696 "gram.in"
{ exio(yypvt[-0], 0); } break;
case 185:
# line 698 "gram.in"
{ exnull(); } break;
case 186:
# line 700 "gram.in"
{
		exnull();
		putsii(ICCOMMENT, yypvt[-0]);
		cfree(yypvt[-0]);
		exnull();
		} break;
case 187:
# line 709 "gram.in"
{ TEST fprintf(diagfile, "exec:empty\n"); } break;
case 188:
# line 711 "gram.in"
{ TEST fprintf(diagfile, "exec: continue\n"); } break;
case 189:
# line 715 "gram.in"
{
		thisexec->copylab = 1;
		++blklevel;
		dclsect = 1;
		ndecl[blklevel] = 0;
		nhid [blklevel] = 0;
		} break;
case 190:
# line 725 "gram.in"
{
		if(ndecl[blklevel]) unhide();
		--blklevel;
		dclsect = 0;
		} break;
case 191:
# line 733 "gram.in"
{ yyval = mklabel(yypvt[-2],1); } break;
case 192:
# line 735 "gram.in"
{ yyval = mklabel(mkilab(yypvt[-1]),1); } break;
case 193:
# line 737 "gram.in"
{ yyval = mklabel(mkilab(yypvt[-3]),1); } break;
case 194:
# line 739 "gram.in"
{ brkcase(); mkcase(0,1); } break;
case 195:
# line 740 "gram.in"
{ brkcase(); } break;
case 197:
# line 744 "gram.in"
{ mkcase(yypvt[-0],1); } break;
case 198:
# line 746 "gram.in"
{ mkcase(yypvt[-0],1); } break;
case 199:
# line 750 "gram.in"
{ TEST fprintf(diagfile, "if-then-else\n");
		  i = yypvt[-5]->brnchend & yypvt[-0]->brnchend;
		  addexec();
		  yyval = addexec();
		  thisexec->brnchend = i;
		  TEST fprintf(diagfile, "exec: if(expr) exec else exec\n"); } break;
case 200:
# line 757 "gram.in"
{ TEST fprintf(diagfile, "if-then\n");
		  pushlex = 1;
		  yyclearin;
		  yyval = ifthen();
		  TEST fprintf(diagfile, "exec: if(expr) exec\n"); } break;
case 201:
# line 763 "gram.in"
{ TEST fprintf(diagfile, "repeat done\n"); } break;
case 202:
# line 765 "gram.in"
{ TEST fprintf(diagfile, "exec: control exec\n"); yyval = addexec(); } break;
case 203:
# line 769 "gram.in"
{ pushctl(STIF,yypvt[-1]); } break;
case 204:
# line 773 "gram.in"
{
		if(thisctl->breaklab == 0)
			thisctl->breaklab = nextlab();
		/* if(thisexec->prevexec->brnchend == 0) */
			exgoto(thisctl->breaklab);
		exlab( indifs[thisctl->indifn] = nextlab() );
		} break;
case 205:
# line 783 "gram.in"
{ pushctl(STREPEAT, 0); } break;
case 206:
# line 787 "gram.in"
{ pushlex = 1;
		  yyclearin;
		  yyval = addexec();
		  exgoto(thisctl->xlab);
		  TEST fprintf(diagfile, " no until\n"); } break;
case 207:
# line 793 "gram.in"
{ yyval = addexec();
		  exnull();
		  exlab( indifs[thisctl->indifn] = nextlab() );
		  exifgo(mknode(TNOTOP,OPNOT,yypvt[-1],0), thisctl->xlab);
		  TEST fprintf(diagfile, "until\n");
		} break;
case 208:
# line 802 "gram.in"
{ exretn(0); } break;
case 209:
# line 804 "gram.in"
{ exretn(yypvt[-0]); } break;
case 211:
# line 807 "gram.in"
{ exgoto(yypvt[-0]); } break;
case 212:
# line 809 "gram.in"
{ if( !equals(yypvt[-1]->namep, "to") )
			execerr("go %s ??\n", yypvt[-1]->namep);
		  else	 exgoto(yypvt[-0]);
		} break;
case 213:
# line 814 "gram.in"
{ excompgoto(yypvt[-1], yypvt[-0]); } break;
case 214:
# line 816 "gram.in"
{ if(!equals(yypvt[-2]->namep, "to") )
			execerr("go %s ??\n", yypvt[-2]->namep);
		  else excompgoto(yypvt[-1], yypvt[-0]);
		} break;
case 215:
# line 823 "gram.in"
{ yyval = yypvt[-1]; } break;
case 216:
# line 828 "gram.in"
{ yyval = mkchain(yypvt[-0],NULL); } break;
case 217:
# line 830 "gram.in"
{ yyval = hookup(yypvt[-2], mkchain(yypvt[-0],NULL) ); } break;
case 219:
# line 835 "gram.in"
{ yyval = yypvt[-0]; } break;
case 220:
# line 840 "gram.in"
{ yyval = mklabel(yypvt[-0],0); } break;
case 221:
# line 842 "gram.in"
{ yyval = mklabel(mkilab(yypvt[-0]),0); } break;
case 222:
# line 844 "gram.in"
{ yyval = mkcase(yypvt[-0],0); } break;
case 223:
# line 846 "gram.in"
{ yyval = mkcase(0,0); } break;
case 224:
# line 850 "gram.in"
{ exbrk(yypvt[-0],0,0); } break;
case 225:
# line 852 "gram.in"
{ exbrk(yypvt[-1],yypvt[-0],0); } break;
case 226:
# line 854 "gram.in"
{ exbrk(yypvt[-1],0,yypvt[-0]); } break;
case 227:
# line 856 "gram.in"
{ exbrk(yypvt[-2],yypvt[-1],yypvt[-0]); } break;
case 228:
# line 858 "gram.in"
{ exbrk(yypvt[-2],yypvt[-0],yypvt[-1]); } break;
case 229:
# line 861 "gram.in"
{ yyval = 1; } break;
case 230:
# line 862 "gram.in"
{ yyval = 0; } break;
case 231:
# line 863 "gram.in"
{ yyval = 0; } break;
case 232:
# line 866 "gram.in"
{ yyval = STWHILE; } break;
case 233:
# line 867 "gram.in"
{ yyval = STFOR; } break;
case 234:
# line 868 "gram.in"
{ yyval = STDO; } break;
case 235:
# line 869 "gram.in"
{ yyval = STREPEAT; } break;
case 236:
# line 870 "gram.in"
{ yyval = STSWITCH; } break;
case 237:
# line 871 "gram.in"
{ yyval = STPROC; } break;
case 238:
# line 875 "gram.in"
{ pushctl(STWHILE, yypvt[-1]);
		    TEST fprintf(diagfile, "while(expr)\n"); } break;
case 239:
# line 878 "gram.in"
{ exlab(thisctl->xlab);
		  if(yypvt[-1])
			exifgo(mknode(TNOTOP,OPNOT,yypvt[-1],0),
				thisctl->breaklab = nextlab() );
		  else exnull();
		  TEST fprintf(diagfile, "for (forlist)\n"); } break;
case 240:
# line 885 "gram.in"
{ pushctl(STSWITCH,  simple(LVAL,yypvt[-1]));
		  TEST fprintf(diagfile, "switch (expr)\n"); } break;
case 241:
# line 888 "gram.in"
{ pushctl(STDO, yypvt[-0]);
		  TEST fprintf(diagfile, "do loop\n"); } break;
case 242:
# line 893 "gram.in"
{ yyval = doloop(yypvt[-2],0,0); } break;
case 243:
# line 895 "gram.in"
{ yyval = doloop(yypvt[-1],0,0); } break;
case 244:
# line 897 "gram.in"
{ yyval = doloop(yypvt[-4],yypvt[-2],0); } break;
case 245:
# line 899 "gram.in"
{ yyval = doloop(yypvt[-3],yypvt[-1],0); } break;
case 246:
# line 901 "gram.in"
{ yyval = doloop(yypvt[-6],yypvt[-4],yypvt[-2]); } break;
case 247:
# line 903 "gram.in"
{ yyval = doloop(yypvt[-5],yypvt[-3],yypvt[-1]); } break;
case 248:
# line 907 "gram.in"
{ pushctl(STFOR,0); } break;
case 249:
# line 911 "gram.in"
{ exgoto(thisctl->xlab);
		  exlab(thisctl->nextlab);
		  addexec();
		  } break;
case 250:
# line 916 "gram.in"
{ exgoto(thisctl->xlab);
		  exlab(thisctl->nextlab);
		  addexec();
		  } break;
case 252:
# line 926 "gram.in"
{ addexec(); } break;
case 253:
# line 928 "gram.in"
{ addexec(); } break;
case 254:
# line 931 "gram.in"
{ yyval = 0; } break;
case 256:
# line 936 "gram.in"
{ if(dbgopt) ++dbglevel; } break;
case 257:
# line 940 "gram.in"
{ if(dbgopt) --dbglevel; } break;
case 258:
# line 944 "gram.in"
{ yyval = mkiost(yypvt[-5], yypvt[-3], yypvt[-1]); } break;
case 259:
# line 947 "gram.in"
{ yyval = 0; } break;
case 260:
# line 948 "gram.in"
{ yyval = 1; } break;
case 261:
# line 949 "gram.in"
{ yyval = 2; } break;
case 262:
# line 950 "gram.in"
{ yyval = 3; } break;
case 264:
# line 954 "gram.in"
{ yyval = NULL; } break;
case 265:
# line 957 "gram.in"
{ yyval = mkchain(yypvt[-0],NULL); } break;
case 266:
# line 958 "gram.in"
{ hookup(yypvt[-2], mkchain(yypvt[-0],NULL)); } break;
case 267:
# line 961 "gram.in"
{ yyval = mkioitem(yypvt[-0],0); } break;
case 268:
# line 962 "gram.in"
{ yyval = mkioitem(yypvt[-2],yypvt[-0]); } break;
case 269:
# line 963 "gram.in"
{ yyval = mkioitem(0,yypvt[-0]); } break;
case 270:
# line 964 "gram.in"
{ yyval = mkiogroup(yypvt[-0], NULL, NULL); } break;
case 271:
# line 965 "gram.in"
{ yyval = mkiogroup(yypvt[-0], NULL, yypvt[-1]); } break;
case 272:
# line 966 "gram.in"
{ yyval = mkiogroup(yypvt[-2],yypvt[-0],yypvt[-3]); } break;
case 273:
# line 967 "gram.in"
{ yyval = mkiogroup(yypvt[-2],yypvt[-0],NULL); } break;
case 274:
# line 970 "gram.in"
{ ++iobrlevel; } break;
case 275:
# line 971 "gram.in"
{ --iobrlevel;  yyval = yypvt[-1]; } break;
case 276:
# line 975 "gram.in"
{ yyval = mkformat(yypvt[-0],0,0); } break;
case 277:
# line 977 "gram.in"
{ yyval = mkformat(yypvt[-3],yypvt[-1],0); } break;
case 278:
# line 979 "gram.in"
{ yyval = mkformat(yypvt[-5],yypvt[-3],yypvt[-1]); } break;
case 279:
# line 981 "gram.in"
{ yyval = yypvt[-1]->leftp; frexpblock(yypvt[-1]); } break;
case 280:
# line 984 "gram.in"
{ yyval = yypvt[-1]; } break;
case 281:
# line 987 "gram.in"
{ lettneed = YES;} break;
case 282:
# line 990 "gram.in"
{ lettneed = NO; } break;
		}
		goto yystack;  /* stack new state and value */

	}
