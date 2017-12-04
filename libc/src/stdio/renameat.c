#include <stdio.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_RENAMEAT, int, renameat, (int, const char*, int, const char*));
