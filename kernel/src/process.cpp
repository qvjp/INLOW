#include <errno.h>
#include <string.h>
#include <inlow/kernel/elf.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/physicalmemory.h>
#include <inlow/kernel/process.h>
#include <inlow/kernel/terminal.h>

Process* Process::current;
static Process* firstProcess;
static Process* idleProcess;

Process::Process()
{
	addressSpace = kernelSpace;
	interruptContext = nullptr;
	prev = nullptr;
	next = nullptr;
	kernelStack = nullptr;
	memset(fd, 0, sizeof(fd));
	rootFd = nullptr;
	cwdFd = nullptr;
}

void Process::initialize(FileDescription* rootFd)
{
	idleProcess = new Process();
	idleProcess->interruptContext = new InterruptContext();
	idleProcess->rootFd = rootFd;
	current = idleProcess;
	firstProcess = nullptr;
}

Process* Process::loadELF(vaddr_t elf)
{
	ELFHeader* header = (ELFHeader*)elf;
	ProgramHeader* programHeader = (ProgramHeader*) (elf + header->e_phoff);

	AddressSpace* addressSpace = kernelSpace->fork();
	for (size_t i = 0; i < header->e_phnum; i++)
	{
		if (programHeader[i].p_type != PT_LOAD)
				continue;
		vaddr_t loadAddressAligned = programHeader[i].p_paddr & ~0xFFF;
		ptrdiff_t offset = programHeader[i].p_paddr - loadAddressAligned;

		const void* src = (void*) (elf + programHeader[i].p_offset);

		size_t size = ALIGNUP(programHeader[i].p_memsz + offset, 0x1000);
		addressSpace->mapMemory(loadAddressAligned, size, PROT_READ | PROT_WRITE | PROT_EXEC);
		vaddr_t dest = kernelSpace->mapFromOtherAddressSpace(addressSpace, loadAddressAligned, size, PROT_WRITE);
		memset((void*) (dest + offset), 0 ,programHeader[i].p_memsz);
		memcpy((void*) (dest + offset), src, programHeader[i].p_filesz);
		kernelSpace->unmapPhysical(dest, size);
	}
	return startProcess((void*) header->e_entry, addressSpace);
}

InterruptContext* Process::schedule(InterruptContext* context)
{
	current->interruptContext = context;

	if (current->next)
	{
		current = current->next;
	}
	else
	{
		if (firstProcess)
		{
			current = firstProcess;
		}
		else
		{
			current = idleProcess;
		}
	}
	setKernelStack((uintptr_t) current->kernelStack + 0x1000);

	current->addressSpace->activate();
	return current->interruptContext;
}

Process* Process::startProcess(void* entry, AddressSpace* addressSpace)
{
	Process* process = new Process();
	vaddr_t stack = addressSpace->mapMemory(0x1000, PROT_READ | PROT_WRITE);
	process->kernelStack = (void*) kernelSpace->mapMemory(0x1000, PROT_READ | PROT_WRITE);

	process->interruptContext = (InterruptContext*) ((uintptr_t)process->kernelStack + 0x1000 - sizeof(InterruptContext));

	process->interruptContext->eax = 0;
	process->interruptContext->ebx = 0;
	process->interruptContext->ecx = 0;
	process->interruptContext->edx = 0;
	process->interruptContext->esi = 0;
	process->interruptContext->edi = 0;
	process->interruptContext->ebp = 0;
	process->interruptContext->interrupt = 0;
	process->interruptContext->error = 0;
	process->interruptContext->eip = (uint32_t)entry;
	process->interruptContext->cs = 0x1B;
	process->interruptContext->eflags = 0x200;//Interrupt enable
	process->interruptContext->esp = (uint32_t) stack + 0x1000;
	process->interruptContext->ss = 0x23;

	process->addressSpace = addressSpace;

	// Initialize file descriptors
	process->fd[0] = new FileDescription(&terminal); //stdin
	process->fd[1] = new FileDescription(&terminal); //stdout
	process->fd[2] = new FileDescription(&terminal); //stderr

	process->rootFd = new FileDescription(*idleProcess->rootFd);
	process->cwdFd = new FileDescription(*process->rootFd);

	process->next = firstProcess;
	if (process->next)
	{
		process->next->prev = process;
	}
	firstProcess = process;

	return process;
}

void Process::exit(int status)
{
	if (next)
	{
		next->prev = prev;
	}
	if (prev)
	{
		prev->next = next;
	}
	if (this == firstProcess)
	{
		firstProcess = next;
	}
	// Clean UP
	delete addressSpace;
	for (size_t i = 0; i < 20; i++)
			if (fd[i])
					delete fd[i];
	delete rootFd;
	delete cwdFd;
	Print::printf("Process exited with status %u\n", status);
}

int Process::registerFileDescriptor(FileDescription* descr)
{
	for (int i = 0; i< 20; i++)
	{
		if (fd[i] == nullptr)
		{
			fd[i] = descr;
			return i;
		}
	}
	errno = EMFILE;
	return -1;
}
