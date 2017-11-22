#include <assert.h>
#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/kthread.h>
#include <inlow/kernel/print.h>

static kthread_mutex_t heapLock = KTHREAD_MUTEX_INITIALIZER;

extern "C" void __assertionFailure(const char* assertion, const char* file, unsigned int line, const char* func)
{
	Print::printf("Assertion failed: '%s' at function %s (%s:%u)\n",assertion, func, file, line);
	while (true)
			asm volatile("cli; hlt");
}

extern "C" void __lockHeap(void)
{
	kthread_mutex_lock(&heapLock);
}

extern "C" void* __mapMemory(size_t size)
{
	return (void*) kernelSpace->mapMemory(size, PROT_READ | PROT_WRITE);
}

extern "C" void __unlockHeap(void)
{
	kthread_mutex_unlock(&heapLock);
}

extern "C" void __unmapMemory(void* addr, size_t size)
{
	kernelSpace->unmapMemory((vaddr_t) addr, size);
}
