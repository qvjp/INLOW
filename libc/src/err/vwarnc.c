#include <err.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void vwarnc(int errnum, const char* format, va_list ap)
{
	fputs(program_invocation_name, stderr);
	fputs(": ", stderr);
	if (format)
	{
		vfprintf(stderr, format, ap);
		fputs(": ", stderr);
	}
	fputs(strerror(errnum), stderr);
	fputc('\n', stderr);
}
