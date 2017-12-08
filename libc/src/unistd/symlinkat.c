#include <unistd.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_SYMLINKAT, int, symlinkat, (const char*, int, const char*));
