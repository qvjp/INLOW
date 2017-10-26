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
			~Process();
			void exit(int status);
			Process* regfork(int flags, struct regfork* registers);
			int execute(FileDescription* descr, char* const argv[], char* const envp[]);
			int registerFileDescriptor(FileDescription* descr);
			Process* waitpid(pid_t pid, int flags);
	private:
			int copyArguments(char* const argv[], char* const envp[], char**& newArgv, char**& newEnvp);
			uintptr_t loadELF(uintptr_t elf);
	private:
			InterruptContext* interruptContext;
			Process* prev;
			Process* next;
			void* kernelStack;
			bool contextChanged;
			bool fdInitialized;
			bool terminated;
			Process* parent;
			Process** children;
			size_t numChildren;

	public:
			AddressSpace* addressSpace;
			FileDescription* fd[OPEN_MAX];
			FileDescription* rootFd;
			FileDescription* cwdFd;
			pid_t pid;
			int status;
	public:
			static void addProcess(Process* process);
			static void initialize(FileDescription* rootFd);
			static InterruptContext* schedule(InterruptContext* context);
			static Process* current;
};

void setKernelStack(uintptr_t stack);

#endif
