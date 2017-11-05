
#include <err.h>
#include <errno.h>
#include <stdlib.h>

__noreturn void verrc(int status, int errnum, const char* format, va_list ap)
{
	vwarnc(errnum, format, ap);
	exit(status);
}
