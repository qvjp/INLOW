#include <stdio.h>

int puts(const char* s)
{
	flockfile(stdout);
	while (*s)
	{
		putchar_unlocked(*s++);
	}

	putchar_unlocked('\n');
	funlockfile(stdout);
	return 1;
}
