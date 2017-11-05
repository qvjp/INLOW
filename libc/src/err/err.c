#include <err.h>
#include <stdarg.h>

__noreturn void err(int status, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	verr(status, format, ap);
}
