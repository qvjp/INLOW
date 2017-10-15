#include <unistd.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_WRITE, ssize_t, write, (int, const void*, size_t));
