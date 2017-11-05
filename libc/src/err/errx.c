#include <err.h>
#include <stdarg.h>

__noreturn void errx(int status, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	verrx(status, format, ap);
}
