#include <signal.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_SIGACTION, int, sigaction,
		(int, const struct sigaction* restrict, struct sigaction* restrict));
