#include <sys/stat.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_FSTAT, int, fstat, (int, struct stat*));
