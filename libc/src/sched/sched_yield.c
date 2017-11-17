#include <sched.h>

int sched_yield(void)
{
	#ifdef __i386__
		asm volatile("int $0x31");
	#else
	#error "sched_yield is unimplememted for this architecture."
	#endif
		return 0;
}
