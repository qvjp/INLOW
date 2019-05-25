#include <stdlib.h>
#include <unistd.h>

__dead void _Exit(int status)
{
    _exit(status);
}
