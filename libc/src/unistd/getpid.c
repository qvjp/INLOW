#include <unistd.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_GETPID, pid_t, getpid, (void));
