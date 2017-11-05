
#include <err.h>
#include <errno.h>
#include <stdarg.h>

void vwarn(const char* format, va_list ap)
{
	vwarnc(errno, format, ap);
}
