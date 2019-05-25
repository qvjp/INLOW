#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

static size_t callback(void* file, const char* s, size_t nBytes) {
    FILE* f = (FILE*) file;
    return write(f->fd, s, nBytes);
}

int vfprintf_unlocked(FILE* restrict file, const char* restrict format,
        va_list ap) {
    return vcbprintf(file, callback, format, ap);
}