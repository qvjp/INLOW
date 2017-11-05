#include <err.h>
#include <stdarg.h>

__noreturn void errc(int status, int errnum, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	verrc(status, errnum, format, ap);
}
