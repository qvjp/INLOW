#include <err.h>
#include <stdarg.h>

void warnx(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	vwarnx(format, ap);
	va_end(ap);
}
