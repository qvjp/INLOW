#include <string.h>
#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/directory.h>
#include <inlow/kernel/file.h>
#include <inlow/kernel/initrd.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/physicalmemory.h>
#include <inlow/kernel/pit.h>
#include <inlow/kernel/process.h>
#include <inlow/kernel/ps2.h>

#ifndef INLOW_VERSION
#define INLOW_VERSION ""
#endif

static DirectoryVnode* loadInitrd(multiboot_info* multiboot);

extern "C" void kernel_main(uint32_t, paddr_t multibootAddress)
{
		Print::printf("Welcome to INLOW " INLOW_VERSION "\n");
		Print::printf("Initializing Address space...\n");
		AddressSpace::initialize();

		Print::printf("Initializing Physical Memory...\n");
		multiboot_info* multiboot = (multiboot_info*) kernelSpace->mapPhysical(
						multibootAddress, 0x1000, PROT_READ);

		PhysicalMemory::initialize(multiboot);

		Print::printf("Initializing PS/2 Controller...\n");
		PS2::initialize();

		// Load the initrd
		Print::printf("Loading Initrd...\n");
		DirectoryVnode* rootDir = loadInitrd(multiboot);
		FileDescription* rootFd = new FileDescription(rootDir);
		
		Print::printf("Initializing process...\n");
		Process::initialize(rootFd);
		FileVnode* program = (FileVnode*) rootDir->openat("/bin/sh", 0, 0);
		if (program)
		{
				Process* newProcess = new Process();
				const char* argv[] = { "/bin/sh", nullptr };
				const char* envp[] = { "PATH=/bin", nullptr };
				newProcess->execute(new FileDescription(program), (char**) argv, (char**) envp);
				Process::addProcess(newProcess);
		}

		kernelSpace->unmapPhysical((vaddr_t) multiboot, 0x1000);


		Print::printf("Enabling interrupts...\n");
		Interrupts::initPic();
		Pit::initialize();
		Interrupts::enable();
		Print::printf("Initialization completed.\n");

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
