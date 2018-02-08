#include <assert.h>
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
#include <inlow/kernel/rtc.h>

#ifndef INLOW_VERSION
#define INLOW_VERSION ""
#endif

static Reference<DirectoryVnode> loadInitrd(multiboot_info* multiboot);

static multiboot_info multiboot;

extern "C" void kernel_main(uint32_t, paddr_t multibootAddress)
{
		Print::printf("Welcome to INLOW " INLOW_VERSION "\n");
		// Print::printf("Initializing Address space...\n");
		AddressSpace::initialize();

		multiboot_info* multibootMapped = (multiboot_info*) kernelSpace->mapPhysical(
						multibootAddress, 0x1000, PROT_READ);

		memcpy(&multiboot, multibootMapped, sizeof(multiboot_info));
		kernelSpace->unmapPhysical((vaddr_t) multibootMapped, 0x1000);

		// Print::printf("Initializing Physical Memory..\n");
		PhysicalMemory::initialize(&multiboot);

		// Print::printf("Initializing PS/2 Controller...\n");
		PS2::initialize();

		// Load the initrd
		// Print::printf("Loading Initrd...\n");
		Reference<DirectoryVnode> rootDir = loadInitrd(&multiboot);
		FileDescription* rootFd = new FileDescription(rootDir);
		
		// Print::printf("Initializing process...\n");
		Process::initialize(rootFd);
		Reference<Vnode> program = resolvePath(rootDir, "/sbin/init");
		if (program)
		{
				Process* newProcess = new Process();
				const char* argv[] = { "init", nullptr };
				const char* envp[] = { nullptr };
				newProcess->execute(program, (char**) argv, (char**) envp);
				Process::addProcess(newProcess);
				assert(newProcess->pid == 1);
				Process::initProcess = newProcess;
		}


		// Print::printf("Enabling interrupts...\n");
		Interrupts::initPic();
		Rtc::initialize();
		Pit::initialize();
		// Print::printf("Initialization completed.\n");
		Interrupts::enable();

		while (true)
		{
			asm volatile ("hlt");
		}

}

static Reference<DirectoryVnode> loadInitrd(multiboot_info* multiboot)
{
		Reference<DirectoryVnode> root;
		paddr_t modulesAligned = multiboot->mods_addr & ~0xFFF;
		ptrdiff_t offset = multiboot->mods_addr - modulesAligned;
		size_t mappedSize = ALIGNUP(offset + multiboot->mods_count * sizeof(multiboot_mod_list), 0x1000);

		vaddr_t modulesPage = kernelSpace->mapPhysical(modulesAligned, mappedSize, PROT_READ);

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
		kernelSpace->unmapPhysical((vaddr_t) modulesPage, mappedSize);
		return root;
}
