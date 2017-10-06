#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/physicalmemory.h>
#include <inlow/kernel/process.h>

static void processA()
{
	while(true)
			Print::printf("A");
}
static void processB()
{
	while(true)
			Print::printf("B");
}

extern "C" void kernel_main(uint32_t, paddr_t multibootAddress)
{
		Print::printf("Hello World! I'm INLOW\n");
		AddressSpace::initialize();
		Print::printf("Address space initialized!\n");

		multiboot_info* multiboot = (multiboot_info*) kernelSpace->map(
						multibootAddress, PAGE_PRESENT | PAGE_WRITABLE);

		PhysicalMemory::initialize(multiboot);
		Print::printf("Physical Memory initialized\n");

		kernelSpace->unmap((vaddr_t) multiboot);

		Process::initialize();
		Process::startProcess((void*) processA);
		Process::startProcess((void*) processB);
		Print::printf("Processes initialized\n");


		Interrupts::initPic();
		Interrupts::enable();
		Print::printf("Interrupts enable!\n");

		while (true)
		{
			asm volatile ("hlt");
		}

}
