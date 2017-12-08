#include <unistd.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_LINKAT, int, linkat, (int, const char*, int, const char*, int));
