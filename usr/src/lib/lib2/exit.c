static char	sccsid[] = "@(#)exit.c	4.1";

exit ()
{
	_cleanup ();
	_exit ();
}
