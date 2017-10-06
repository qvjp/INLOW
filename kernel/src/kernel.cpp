#include <string.h>
#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/physicalmemory.h>
#include <inlow/kernel/process.h>

static void processA()
{
		asm volatile ("int $0x30" :: "a"(0), "b"(0));
		__builtin_unreachable();
}
static void processB()
{
		asm volatile ("int $0x30" :: "a"(0), "b"(42));
		__builtin_unreachable();
}
static Process* startProcesses(void* function)
{
	AddressSpace* addressSpace = kernelSpace->fork();
	paddr_t phys = PhysicalMemory::popPageFrame();
	void* processCode = (void*) addressSpace->map(phys, PAGE_PRESENT | PAGE_USER);
	vaddr_t processMapped = kernelSpace->map(phys, PAGE_PRESENT | PAGE_WRITABLE);
	memcpy((void*) processMapped, function, 0x1000);
	kernelSpace->unmap(processMapped);
	return Process::startProcess(processCode, addressSpace);
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
		startProcesses((void*) processA);
		startProcesses((void*) processB);

		Print::printf("Processes initialized\n");


		Interrupts::initPic();
		Interrupts::enable();
		Print::printf("Interrupts enable!\n");

		while (true)
		{
			asm volatile ("hlt");
		}

}
