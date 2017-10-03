#ifndef KERNEL_PHYSICALMEMORY_H
#define KERNEL_PHYSICALMEMORY_H

#include <inlow/kernel/multiboot.h>

namespace PhysicalMemory
{
	void initialize(multiboot_info* multiboot);
	void pushPageFrame(paddr_t physicalAddress);
	paddr_t popPageFrame();
}

#endif
