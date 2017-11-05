
#include <err.h>
#include <errno.h>
#include <stdlib.h>

__noreturn void verrx(int status, const char* format, va_list ap)
{
	vwarnx(format, ap);
	exit(status);
}
