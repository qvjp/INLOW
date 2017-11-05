#include <err.h>
#include <stdarg.h>

void warn(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	vwarn(format, ap);
	va_end(ap);
}
