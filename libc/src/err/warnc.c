#include <err.h>
#include <stdarg.h>

void warnc(int errnum, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	vwarnc(errnum, format, ap);
	va_end(ap);
}
