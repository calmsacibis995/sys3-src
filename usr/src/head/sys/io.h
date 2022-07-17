#include "sys/tty.h"
#include "sys/peri.h"


#ifdef RP03_0
struct {
	daddr_t	nblocks;
	int	cyloff;
} rp_sizes[8] = {
	10000,	0,		/* cyl 0 thru 49 */
	71200,	50,		/* cyl 50 thru 405 */
	40600,	203,		/* cyl 203 thru 405 */
	0,	0,
	0,	0,
	0,	0,
	0,	0,
	81200,	0,		/* cyl 0 thru 405 */
};
#endif
#ifdef RP05_0
#define RP04_0
#endif
#ifdef RP04_0
struct {
	daddr_t	nblocks;
	int	cyloff;
} hp_sizes[8] = {
	18392,	0,		/* cyl 0 thru 43 */
	153406,	44,		/* cyl 44 thru 410 */
	87780,	201,		/* cyl 201 thru 410 */
	22154,	358,		/* cyl 358 thru 410 */
	0,	0,
	0,	0,
	0,	0,
	171798,	0,		/* cyl 0 thru 410 */
};
#endif
#ifdef RP06_0
struct {
	daddr_t	nblocks;
	int	cyloff;
} hp_sizes[8] = {
	18392,	0,		/* cyl 0 thru 43 */
	322278,	44,		/* cyl 44 thru 814 */
	256652,	201,		/* cyl 201 thru 814 */
	191026,	358,		/* cyl 358 thru 814 */
	125400,	515,		/* cyl 515 thru 814 */
	59774,	672,		/* cyl 672 thru 814 */
	0,	0,
	340670,	0,		/* cyl 0 thru 814 */
};
#endif
