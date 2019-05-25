#include <stdlib.h>

extern void _fini(void);

__dead void exit(int status)
{
    _fini();
    _Exit(status);
}
