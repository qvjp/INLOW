#include <stdio.h>

static inline int puts_unlocked(const char* s)
{
	while (*s)
	{
		if (putchar_unlocked(*s++) < 0)
				return EOF;
	}
	if (putchar_unlocked('\n') < 0)
			return EOF;
	return 1;
}

int puts(const char* s)
{
	flockfile(stdout);
	int result = puts_unlocked(s);
	funlockfile(stdout);
	return result;
}
