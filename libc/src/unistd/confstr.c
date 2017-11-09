#include <unistd.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_CONFSTR, size_t, confstr, (int, char*, size_t));
