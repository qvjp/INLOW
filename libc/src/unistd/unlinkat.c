#include <unistd.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_UNLINKAT, int, unlinkat, (int, const char*, int));
