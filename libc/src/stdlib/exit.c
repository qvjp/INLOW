#include <stdlib.h>

extern void _fini(void);

__noreturn void exit(int status)
{
	_fini();
	_Exit(status);
}
