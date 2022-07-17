#include "sys/param.h"
#include "sys/systm.h"
#include "sys/proc.h"


pwr()
{
	register (**clr)();
	register	s;

	s = spl1();
	printf("\377\377\377\n\nPower fail #%d\n\n", pwr_cnt);
	pwr_act = 1;
/*
 * clear adaptors 
*/
	mbaclr();
	ubaclr();
	/* call clear and start routines */
	clr = &pwr_clr[0];
	while (*clr) {
		(**clr)();
		clr++;
	}
	/* if none - halt */
	if (clr == &pwr_clr[0]) {
		printf("Stopped\n");
		for (;;);
	}
	/* tell Init */
	psignal(&proc[1], SIGHUP);
	pwr_act = 0;
	splx(s);
}
