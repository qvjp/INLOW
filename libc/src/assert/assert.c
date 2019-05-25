#include <assert.h>
#include <stdio.h>
#include <unistd.h>

__dead void __assertionFailure(const char* assertion, const char* file,
        unsigned int line, const char* func) {
    fprintf(stderr, "Assertion failed: '%s' at function %s (%s:%u)\n",
            assertion, func, file, line);
    _exit(1);
}