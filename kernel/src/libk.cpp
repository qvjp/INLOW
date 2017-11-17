#include <assert.h>
#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/print.h>

extern "C" void __assertionFailure(const char* assertion, const char* file, unsigned int line, const char* func)
{
	Print::printf("Assertion failed: '%s' at function %s (%s:%u)\n",assertion, func, file, line);
	while (true)
			asm volatile("cli; hlt");
}

extern "C" void* __mapMemory(size_t size)
{
	return (void*) kernelSpace->mapMemory(size, PROT_READ | PROT_WRITE);
}

extern "C" void __unmapMemory(void* addr, size_t size)
{
	kernelSpace->unmapMemory((vaddr_t) addr, size);
}
