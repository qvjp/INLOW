#include <stdlib.h>
#include <sys/types.h>

#define ATEXIT_MAX 32

static void (*atexitHandlers[ATEXIT_MAX])(void);

int atexit(void (*func)(void))
{
	for (size_t i = 0; i < ATEXIT_MAX; i++)
	{
		if (!atexitHandlers[i])
		{
			atexitHandlers[i] = func;
			return 0;
		}
	}
	return -1;
}

__attribute__((destructor))
		void __callAtexitHandler(void)
{
	for (ssize_t i = ATEXIT_MAX - 1; i >= 0; i--)
	{
		if (atexitHandlers[i])
				atexitHandlers[i]();
	}
}
