#include <err.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

void vwarnx(const char* format, va_list ap)
{
	fputs(program_invocation_name, stderr);
	fputs(": ", stderr);
	if (format)
	{
		vfprintf(stderr, format, ap);
	}
	fputc('\n', stderr);
}
