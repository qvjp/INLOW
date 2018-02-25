#include <time.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_CLOCK_NANOSLEEP, int, clock_nanosleep, (clockid_t, int, const struct timespec*, struct timespec*));
