#ifndef KERNEL_PROCESS_H
#define KERNEL_PROCESS_H

#include <sys/types.h>
#include <inlow/fork.h>
#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/filedescription.h>
#include <inlow/kernel/interrupts.h>

#define OPEN_MAX 20

class Process
{
	public:
			Process();
			void exit(int status);
			Process* regfork(int flags, struct regfork* registers);
			int registerFileDescriptor(FileDescription* descr);
	private:
			InterruptContext* interruptContext;
			Process* prev;
			Process* next;
			void* kernelStack;

	public:
			AddressSpace* addressSpace;
			FileDescription* fd[OPEN_MAX];
			FileDescription* rootFd;
			FileDescription* cwdFd;
			pid_t pid;
	public:
			static void initialize(FileDescription* rootFd);
			static Process* loadELF(vaddr_t elf);
			static InterruptContext* schedule(InterruptContext* context);
			static Process* startProcess(void* entry, AddressSpace* addressSpace);
			static Process* current;
};

void setKernelStack(uintptr_t stack);

#endif
