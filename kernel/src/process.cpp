#include <inlow/kernel/print.h>
#include <inlow/kernel/process.h>

Process* Process::current;
static Process* firstProcess;
static Process* idleProcess;

Process::Process()
{
	addressSpace = kernelSpace;
	interruptContext = nullptr;
	prev = nullptr;
	next = nullptr;
	stack = nullptr;
	kernelStack = nullptr;
}

void Process::initialize()
{
	idleProcess = new Process();
	idleProcess->interruptContext = new InterruptContext();
	current = idleProcess;
	firstProcess = nullptr;
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
	process->stack = (void*) addressSpace->allocate(1);
	process->kernelStack = (void*) kernelSpace->allocate(1);

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
	process->interruptContext->esp = (uint32_t) process->stack + 0x1000;
	process->interruptContext->ss = 0x23;

	process->addressSpace = addressSpace;

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
	Print::printf("Process exited with status %u\n", status);
}
