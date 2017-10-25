#include <unistd.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_REGFORK, pid_t, regfork, (int, struct regfork*));
