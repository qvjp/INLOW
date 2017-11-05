#include <err.h>
#include <errno.h>
#include <stdarg.h>

__noreturn void verr(int status, const char* format, va_list ap)
{
	verrc(status, errno, format, ap);
}
