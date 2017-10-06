#include <assert.h>
#include <inlow/kernel/print.h>

extern "C" void __assertionFailure(const char* assertion, const char* file, unsigned int line, const char* func)
{
	Print::printf("Assertion failed: '%s' at function %s (%s:%u)\n",assertion, func, file, line);
	while (true)
			asm volatile("cli; hlt");
}
