#include <stddef.h>
#include <stdint.h>
#include <inlow/kernel/interrupts.h>
#include <inlow/kernel/print.h>

extern "C" void kernel_main(uint32_t, uintptr_t)
{
		char c = 'g';
		char* s = "xiao ming";
		Print::printf("Hello World! I'm INLOW\n");
		Print::printf("This is Numbers: %u, 0x%x\n", 42, 42);
		Print::printf("This is Char: %c\n",c);
		Print::printf("This is String: %s\n",s);
		Print::printf("This is %%\n");
		Print::printf("This is no \0 over\n");

		Interrupts::initPic();
		Interrupts::enable();
		Print::printf("Interrupts enable!\n");

		while (true);

}
