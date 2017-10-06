#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/process.h>

static Process* currentProcess;
static Process* firstProcess;
static Process* idleProcess;

Process::Process()
{
	addressSpace = kernelSpace;
	interruptContext = nullptr;
	next = nullptr;
	stack = nullptr;
	kernelStack = nullptr;
}

void Process::initialize()
{
	idleProcess = new Process();
	idleProcess->interruptContext = new InterruptContext();
	currentProcess = idleProcess;
	firstProcess = nullptr;
}

InterruptContext* Process::schedule(InterruptContext* context)
{
	currentProcess->interruptContext = context;

	if (currentProcess->next)
	{
		currentProcess = currentProcess->next;
	}
	else
	{
		if (firstProcess)
		{
			currentProcess = firstProcess;
		}
		else
		{
			currentProcess = idleProcess;
		}
	}
	setKernelStack(currentProcess->kernelStack);

	currentProcess->addressSpace->activate();
	return currentProcess->interruptContext;
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
	firstProcess = process;

	return process;
}
