#include <time.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_CLOCK_GETTIME, int, clock_gettime, (clockid_t, struct timespec*));
