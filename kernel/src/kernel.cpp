#include <string.h>
#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/directory.h>
#include <inlow/kernel/file.h>
#include <inlow/kernel/initrd.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/physicalmemory.h>
#include <inlow/kernel/process.h>
#include <inlow/kernel/ps2.h>

static DirectoryVnode* loadInitrd(multiboot_info* multiboot);

extern "C" void kernel_main(uint32_t, paddr_t multibootAddress)
{
		Print::printf("Hello World! I'm INLOW\n");
		AddressSpace::initialize();
		Print::printf("Address space initialized!\n");

		multiboot_info* multiboot = (multiboot_info*) kernelSpace->map(
						multibootAddress, PAGE_PRESENT | PAGE_WRITABLE);

		PhysicalMemory::initialize(multiboot);
		Print::printf("Physical Memory initialized\n");

		PS2::initialize();
		Print::printf("PS/2 Controller initialized\n");

		// Load the initrd
		DirectoryVnode* rootDir = loadInitrd(multiboot);
		FileDescription* rootFd = new FileDescription(rootDir);
		Print::printf("Initrd loaded\n");
		
		Process::initialize(rootFd);
		FileVnode* program = (FileVnode*) rootDir->openat("/bin/test", 0, 0);
		if (program)
		{
				Process::loadELF((vaddr_t) program->data);
		}

		Print::printf("Processes initialized\n");
		kernelSpace->unmap((vaddr_t) multiboot);


		Interrupts::initPic();
		Interrupts::enable();
		Print::printf("Interrupts enable!\n");

		while (true)
		{
			asm volatile ("hlt");
		}

}

static DirectoryVnode* loadInitrd(multiboot_info* multiboot)
{
		DirectoryVnode* root = nullptr;
		paddr_t modulesAligned = multiboot->mods_addr & ~0xFFF;
		ptrdiff_t offset = multiboot->mods_addr - modulesAligned;

		vaddr_t modulesPage = kernelSpace->map(modulesAligned, PAGE_PRESENT | PAGE_WRITABLE);

		const multiboot_mod_list* modules = (multiboot_mod_list*)(modulesPage + offset);
		for (size_t i = 0; i < multiboot->mods_count; i++)
		{
			size_t nPages = ALIGNUP(modules[i].mod_end - modules[i].mod_start, 0x1000) / 0x1000;
			vaddr_t initrd  = kernelSpace->mapRange(modules[i].mod_start, nPages, PAGE_PRESENT);
			root = Initrd::loadInitrd(initrd);
			kernelSpace->unmapRange(initrd, nPages);
			if (root->childCount)
					break;
		}
		kernelSpace->unmap((vaddr_t) modulesPage);
		return root;
}
