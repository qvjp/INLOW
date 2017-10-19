#ifndef KERNEL_PROCESS_H
#define KERNEL_PROCESS_H

#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/filedescription.h>
#include <inlow/kernel/interrupts.h>

class Process
{
	public:
			Process();
			void exit(int status);
	private:
			InterruptContext* interruptContext;
			Process* prev;
			Process* next;
			void* stack;
			void* kernelStack;

	public:
			AddressSpace* addressSpace;
			FileDescription* fd[20];
	public:
			static void initialize();
			static Process* loadELF(vaddr_t elf);
			static InterruptContext* schedule(InterruptContext* context);
			static Process* startProcess(void* entry, AddressSpace* addressSpace);
			static Process* current;
};

void setKernelStack(uintptr_t stack);

#endif
