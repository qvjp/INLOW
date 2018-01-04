#ifndef KERNEL_PROCESS_H
#define KERNEL_PROCESS_H

#include <sys/types.h>
#include <inlow/fork.h>
#include <inlow/siginfo.h>
#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/filedescription.h>
#include <inlow/kernel/interrupts.h>
#include <inlow/kernel/kthread.h>

#define OPEN_MAX 20

struct PendingSignal
{
	siginfo_t siginfo;
	PendingSignal* next;
};

class Process
{
	public:
			Process();
			~Process();
			void exit(int status);
			int execute(const Reference<Vnode>& vnode, char* const argv[], char* const envp[]);
			Process* regfork(int flags, struct regfork* registers);
			int registerFileDescriptor(FileDescription* descr);
			Process* waitpid(pid_t pid, int flags);

			InterruptContext* handleSignal(InterruptContext* context);
			void raiseSignal(siginfo_t siginfo);
			void terminateBySignal(siginfo_t siginfo);
			void updatePendingSignals();

	private:
			void terminate();

	private:
			InterruptContext* interruptContext;
			void* kernelStack;
			bool contextChanged;
			bool fdInitialized;
			bool terminated;
			Process* parent;
			Process* firstChild;
			Process* prevChild;
			Process* nextChild;
			kthread_mutex_t childrenMutex;
			PendingSignal* pendingSignals;
			kthread_mutex_t signalMutex;

	public:
			AddressSpace* addressSpace;
			FileDescription* fd[OPEN_MAX];
			FileDescription* rootFd;
			FileDescription* cwdFd;
			pid_t pid;
			mode_t umask;
			siginfo_t terminationStatus;
	public:
			static bool addProcess(Process* process);
			static void initialize(FileDescription* rootFd);
			static InterruptContext* schedule(InterruptContext* context);
			static Process* current;
			static Process* initProcess;
	private:
			int copyArguments(char* const argv[], char* const envp[], char**& newArgv, char**& newEnvp, AddressSpace* newAddressSpace);
			static uintptr_t loadELF(uintptr_t elf, AddressSpace* newAddressSpace);
};

void setKernelStack(uintptr_t stack);

#endif
