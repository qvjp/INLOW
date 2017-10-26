#include <sys/stat.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_FSTATAT, int, fstatat, (int, const char* restrict, struct stat* restrict, int));
