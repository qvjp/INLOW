#include <stdarg.h>
#include <stdio.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/terminal.h>

static size_t callback(void*, const char* s, size_t nBytes);

void Print::printf(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    vcbprintf(nullptr, callback, format, ap);
    va_end(ap);
}

static size_t callback(void*, const char* s, size_t nBytes) {
    return (size_t) terminal.write(s, nBytes);
}

void Print::initTerminal()
{
    Terminal::initTerminal();
}
void Print::warnTerminal()
{
    Terminal::warnTerminal();
}