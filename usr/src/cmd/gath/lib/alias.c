alias(s,t)
char *s,*t;
{
	register char *p,*px,*q;
	q=s;
	for (p=s;*p;p++)
	{
		if (*p=='/') q=p;
	};
	if ((p=(char *)scan("hasp",q)) || (p=(char *)scan("uvac",q)) || (p=(char *)scan("nlsc",q)))
	{
		px=p+4;
		if (*px>='0' && *px<='9') px++;
		for (q=t;p<px;) *q++=(*p++);
		*q=0;
		return (0);
	};
	return (-1);
};
