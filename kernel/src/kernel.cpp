#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/interrupts.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/physicalmemory.h>

extern "C" void kernel_main(uint32_t, paddr_t multibootAddress)
{
		Print::printf("Hello World! I'm INLOW\n");
		AddressSpace::initialize();
		Print::printf("Address space initialized!\n");

		multiboot_info* multiboot = (multiboot_info*) kernelSpace->map(
						multibootAddress, PAGE_PRESENT | PAGE_WRITABLE);

		PhysicalMemory::initialize(multiboot);
		kernelSpace->unmap((vaddr_t) multiboot);

		Interrupts::initPic();
		Interrupts::enable();
		Print::printf("Interrupts enable!\n");

		while (true);

}
