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

		multiboot_info* multiboot = (multiboot_info*) kernelSpace->mapPhysical(
						multibootAddress, 0x1000, PROT_READ);

		PhysicalMemory::initialize(multiboot);
		Print::printf("Physical Memory initialized\n");

		PS2::initialize();
		Print::printf("PS/2 Controller initialized\n");

		// Load the initrd
		DirectoryVnode* rootDir = loadInitrd(multiboot);
		FileDescription* rootFd = new FileDescription(rootDir);
		Print::printf("Initrd loaded\n");
		
		Process::initialize(rootFd);
		FileVnode* program = (FileVnode*) rootDir->openat("/bin/sh", 0, 0);
		if (program)
		{
				Process* newProcess = new Process();
				char* args[] = {nullptr};
				newProcess->execute(new FileDescription(program), args, args);
				Process::addProcess(newProcess);
		}

		Print::printf("Processes initialized\n");
		kernelSpace->unmapPhysical((vaddr_t) multiboot, 0x1000);


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

		vaddr_t modulesPage = kernelSpace->mapPhysical(modulesAligned, 0x1000, PROT_READ);

		const multiboot_mod_list* modules = (multiboot_mod_list*)(modulesPage + offset);
		for (size_t i = 0; i < multiboot->mods_count; i++)
		{
			size_t size = ALIGNUP(modules[i].mod_end - modules[i].mod_start, 0x1000);
			vaddr_t initrd  = kernelSpace->mapPhysical(modules[i].mod_start, size, PROT_READ);
			root = Initrd::loadInitrd(initrd);
			kernelSpace->unmapPhysical(initrd, size);
			if (root->childCount)
					break;
		}
		kernelSpace->unmapPhysical((vaddr_t) modulesPage, 0x1000);
		return root;
}
