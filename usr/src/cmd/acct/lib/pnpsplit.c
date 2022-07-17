/*
 * pnpsplit splits interval into prime & nonprime portions
 * ONLY ROUTINE THAT KNOWS ABOUT HOLIDAYS AND DEFN OF PRIME/NONPRIME
 * MUST BE RECOMPILED AT END OF EACH YEAR (change thisyear and holidays)
 */
#include "acctdef.h"
#include <stdio.h>
#include <time.h>
/*
 * day of year (less 1) for BTL holidays, change elsewhere
 */
static	thisyear = 1980;	/* recompile warning appears aft. Christmas */
static	holidays[] = {
	0,	/* New Year's (Jan 1) */
	48,	/* Wash. Birthday (Feb 18) */
	146,	/* Memorial Day (May 26) */
	185,	/* Indep. Day (Jul 4) */
	244,	/* Labor Day (Sep 1) */
	331,	/* Thanksgiving (Nov 27) */
	332,	/* day after (Nov 28) */
	359,	/* Christmas (Dec 25) */
	360,	/* day after christmas (Dec 26) */
	-1
};

/*
 *	prime(0) and nonprime(1) times during a day
 *	for BTL, prime time is 9AM to 5PM
 */
static struct hours {
	int	h_sec;
	int	h_min;
	int	h_hour;
	int	h_type;		/* prime/nonprime of previous period */
} h[] = {
	0,	0,	9,	NP,	/* 9AM, BTL prime starts */
	0,	0,	17,	P,	/* 5PM, BTL prime ends */
	60,	59,	23,	NP,	/* daysend */
	-1
};
int	daysend[]	= {60, 59, 23};

struct tm *localtime();
long	tmsecs();

/*
 * split interval of length etime, starting at start into prime/nonprime
 * values, return as result
 * input values in seconds
 */
pnpsplit(start, etime, result)
long start, etime, result[2];
{
	struct tm cur, end;
	long tcur, tend;
	long tmp;
	register sameday;
	register struct hours *hp;

	if (thisyear)
		checkhol();

	tcur = start;
	tend = start+etime;
	copyn(&end, localtime(&tend), sizeof(end));
	result[P] = 0;
	result[NP] = 0;

	while (tcur < tend) {	/* one iteration per day or part thereof */
		copyn(&cur, localtime(&tcur), sizeof(cur));
		sameday = cur.tm_yday == end.tm_yday;
		if (ssh(&cur)) {	/* ssh:only NP */
			if (sameday) {
				result[NP] += tend-tcur;
				break;
			} else {
				tmp = tmsecs(&cur, daysend);
				result[NP] += tmp;
				tcur += tmp;
			}
		} else {	/* working day, P or NP */
			for (hp = h; tmless(hp, &cur); hp++);
			for (; hp->h_sec >= 0; hp++) {
				if (sameday && tmless(&end, hp)) {
					result[hp->h_type] = tend-tcur;
					tcur = tend;
					break;	/* all done */
				} else {	/* time to next P/NP change */
					tmp = tmsecs(&cur, hp);
					result[hp->h_type] += tmp;
					tcur += tmp;
					cur.tm_sec = hp->h_sec;
					cur.tm_min = hp->h_min;
					cur.tm_hour = hp->h_hour;
				}
			}
		}
	}
}

/*
 *	starting day after Christmas, complain if holidays not yet updated
 */
checkhol()
{
	register struct tm *tp;
	long t;

	time(&t);
	tp = localtime(&t);
	tp->tm_year += 1900;
	if ((tp->tm_year == thisyear && tp->tm_yday > 359) ||
		tp->tm_year > thisyear)
		fprintf(stderr, "***RECOMPILE pnpsplit WITH NEW HOLIDAYS***\n");
	thisyear = 0;
}

/*
 * ssh returns 1 if Sat, Sun, or Holiday
 */
ssh(ltp)
register struct tm *ltp;
{
	register i;

	if (ltp->tm_wday == 0 || ltp->tm_wday == 6)
		return(1);
	for (i = 0; holidays[i] >= 0; i++)
		if (ltp->tm_yday ==  holidays[i])
			return(1);
	return(0);
}
