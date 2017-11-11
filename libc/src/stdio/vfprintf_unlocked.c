#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

static size_t callback(void* file, const char* s, size_t nBytes)
{
	return fwrite_unlocked(s, 1, nBytes, (FILE*) file);
}

int vfprintf_unlocked(FILE* restrict file, const char* restrict format, va_list ap)
{
	return vcbprintf(file, callback, format, ap);
}
