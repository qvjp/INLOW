#include <stdio.h>

int putchar(int c)
{
	flockfile(stdout);
	int result = putchar_unlocked(c);
	funlockfile(stdout);
	return result;
}
