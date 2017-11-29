#include <sys/stat.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_MKDIRAT, int, mkdirat, (int, const char*, mode_t));
