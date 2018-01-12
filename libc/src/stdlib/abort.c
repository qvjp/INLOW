#include <signal.h>
#include <stdlib.h>
#include <sys/syscall.h>

DEFINE_SYSCALL(SYSCALL_ABORT, __noreturn void, sys_abort, (void));

__noreturn void abort(void)
{
	raise(SIGABRT);
	sys_abort();
}
