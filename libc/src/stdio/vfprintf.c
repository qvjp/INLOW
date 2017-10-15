#include <stdarg.h>
#include <stdio.h>

int vfprintf(FILE* restrict file, const char* restrict format, va_list ap)
{
	flockfile(file);
	int result = vfprintf_unlocked(file, format, ap);
	funlockfile(file);
	return result;
}
