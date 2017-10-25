#include <errno.h>
#include <string.h>
#include <inlow/kernel/elf.h>
#include <inlow/kernel/file.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/physicalmemory.h>
#include <inlow/kernel/process.h>
#include <inlow/kernel/terminal.h>

Process* Process::current;
static Process* firstProcess;
static Process* idleProcess;

static pid_t nextPid = 0;

Process::Process()
{
	addressSpace = nullptr;
	interruptContext = nullptr;
	prev = nullptr;
	next = nullptr;
	kernelStack = nullptr;
	memset(fd, 0, sizeof(fd));
	rootFd = nullptr;
	cwdFd = nullptr;
	pid = nextPid++;
	contextChanged = false;
	fdInitialized = false;
}

void Process::initialize(FileDescription* rootFd)
{
	idleProcess = new Process();
	idleProcess->addressSpace = kernelSpace;
	idleProcess->interruptContext = new InterruptContext();
	idleProcess->rootFd = rootFd;
	current = idleProcess;
	firstProcess = nullptr;
}

void Process::addProcess(Process* process)
{
	process->next = firstProcess;
	if (process->next)
	{
		process->next->prev = process;
	}
	firstProcess = process;
}
uintptr_t Process::loadELF(uintptr_t elf)
{
	ELFHeader* header = (ELFHeader*)elf;
	ProgramHeader* programHeader = (ProgramHeader*) (elf + header->e_phoff);

	if (addressSpace)
			delete addressSpace;
	addressSpace = new AddressSpace();

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
	return (uintptr_t) header->e_entry;
}

InterruptContext* Process::schedule(InterruptContext* context)
{
	if (likely(!current->contextChanged))
	{
		current->interruptContext = context;
	}
	else
	{
		current->contextChanged = false;
	}

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

int Process::execute(FileDescription* descr, char* const [], char* const [])
{
	// load the program
	FileVnode* file = (FileVnode*) descr->vnode;
	uintptr_t entry = loadELF((uintptr_t) file->data);

	vaddr_t stack = addressSpace->mapMemory(0x1000, PROT_READ | PROT_WRITE);
	kernelStack = (void*) kernelSpace->mapMemory(0x1000, PROT_READ | PROT_WRITE);

	interruptContext = (InterruptContext*) ((uintptr_t)kernelStack + 0x1000 - sizeof(InterruptContext));

	memset(interruptContext, 0, sizeof(InterruptContext));

	// TODO: Pass argc, argv and envp to the process
	interruptContext->eip = (uint32_t) entry;
	interruptContext->cs = 0x1B;
	interruptContext->eflags = 0x200;
	interruptContext->esp = (uint32_t) stack + 0x1000;
	interruptContext->ss = 0x23;

	if (!fdInitialized)
	{
		// Initialize file descriptors
		fd[0] = new FileDescription(&terminal); //stdin
		fd[1] = new FileDescription(&terminal); //stdout
		fd[2] = new FileDescription(&terminal); //stderr

		rootFd = new FileDescription(*idleProcess->rootFd);
		cwdFd = new FileDescription(*rootFd);
		fdInitialized = true;
	}

	if (this == current)
	{
		contextChanged = true;
	}

	return 0;
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
	for (size_t i = 0; i < OPEN_MAX; i++)
			if (fd[i])
					delete fd[i];
	delete rootFd;
	delete cwdFd;
	Print::printf("Process %u exited with status %u\n", pid, status);
}

Process* Process::regfork(int, struct regfork* registers)
{
	Process* process = new Process();
	process->kernelStack = (void*) kernelSpace->mapMemory(0x1000, PROT_READ | PROT_WRITE);
	process->interruptContext = (InterruptContext*) ((uintptr_t) process->kernelStack + 0x1000 - sizeof(InterruptContext));
	process->interruptContext->eax = registers->rf_eax;
	process->interruptContext->ebx = registers->rf_ebx;
	process->interruptContext->ecx = registers->rf_ecx;
	process->interruptContext->edx = registers->rf_edx;
	process->interruptContext->esi = registers->rf_esi;
	process->interruptContext->edi = registers->rf_edi;
	process->interruptContext->ebp = registers->rf_ebp;
	process->interruptContext->eip = registers->rf_eip;
	process->interruptContext->esp = registers->rf_esp;

	process->interruptContext->interrupt = 0;
	process->interruptContext->error = 0;
	process->interruptContext->cs = 0x1B;
	process->interruptContext->eflags = 0x200; //Interrupt enable
	process->interruptContext->ss = 0x23;

	// Fork the address space
	process->addressSpace = addressSpace->fork();

	// Fork the file descriptor space;
	for (size_t i = 0; i < OPEN_MAX; i++)
	{
		if (fd[i])
		{
			process->fd[i] = new FileDescription(*fd[i]);
		}
	}

	process->rootFd = new FileDescription(*rootFd);
	process->cwdFd = new FileDescription(*cwdFd);
	process->fdInitialized = true;

	addProcess(process);
	return process;
}
int Process::registerFileDescriptor(FileDescription* descr)
{
	for (int i = 0; i< OPEN_MAX; i++)
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
