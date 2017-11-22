#include <sched.h>
#include <inlow/kernel/kthread.h>

int kthread_mutex_lock(kthread_mutex_t* mutex)
{
	while (__atomic_test_and_set(mutex, __ATOMIC_ACQUIRE))
		sched_yield();
	return 0;
}

int kthread_mutex_unlock(kthread_mutex_t* mutex)
{
	__atomic_clear(mutex, __ATOMIC_RELEASE);
	return 0;
}
