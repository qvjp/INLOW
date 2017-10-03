#include <stddef.h>
#include <stdint.h>
#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/interrupts.h>
#include <inlow/kernel/print.h>

extern "C" void kernel_main(uint32_t, uintptr_t)
{
		Print::printf("Hello World! I'm INLOW\n");
		AddressSpace::initialize();
		Print::printf("Address space initialized!\n");
		Interrupts::initPic();
		Interrupts::enable();
		Print::printf("Interrupts enable!\n");

		while (true);

}
