#include <time.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_NANOSLEEP, int, nanosleep, (const struct timespec*, struct timespec*));
