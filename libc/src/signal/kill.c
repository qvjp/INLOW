#include <signal.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_KILL, int, kill, (pid_t, int));
