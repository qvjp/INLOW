#include <stdio.h>
int getchar(void)
{
	flockfile(stdin);
	int result = getchar_unlocked();
	funlockfile(stdin);
	return result;
}
