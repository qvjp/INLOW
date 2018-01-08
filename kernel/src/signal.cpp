#include <assert.h>
#include <errno.h>
#include <sched.h>
#include <signal.h>
#include <inlow/kernel/interrupts.h>
#include <inlow/kernel/process.h>
#include <inlow/kernel/signal.h>
#include <inlow/kernel/syscall.h>

static sigset_t defaultIgnoredSignals = _SIGSET(SIGCHLD) | _SIGSET(SIGURG);

extern "C"
{
	volatile unsigned long signalPending = 0;
}

static inline bool isMoreImportantSignalThan(int signal1, int signal2)
{
	if (signal1 == SIGKILL)
		return true;
	if (signal2 == SIGKILL)
		return false;
	if (signal1 == SIGSTOP)
		return true;
	if (signal2 == SIGSTOP)
		return false;
	return signal1 <= signal2;
}

extern "C" InterruptContext* handleSignal(InterruptContext* context)
{
	return Process::current->handleSignal(context);
}

InterruptContext* Process::handleSignal(InterruptContext*)
{
	kthread_mutex_lock(&signalMutex);
	assert(pendingSignals);

	PendingSignal* pending = pendingSignals;
	pendingSignals = pending->next;
	siginfo_t siginfo = pending->siginfo;
	delete pending;

	updatePendingSignals();
	kthread_mutex_unlock(&signalMutex);

	assert(!(true && sigismember(&defaultIgnoredSignals, siginfo.si_signo)));

	terminateBySignal(siginfo);
	sched_yield();
	__builtin_unreachable();
}

void Process::raiseSignal(siginfo_t siginfo)
{
	AutoLock lock(&signalMutex);

	if (true && sigismember(&defaultIgnoredSignals, siginfo.si_signo))
	{
		return;
	}

	if (!pendingSignals || isMoreImportantSignalThan(siginfo.si_signo, pendingSignals->siginfo.si_signo))
	{
		if (unlikely(pendingSignals && pendingSignals->siginfo.si_signo == siginfo.si_signo))
		{
			return;
		}
		PendingSignal* pending = new PendingSignal;
		pending->siginfo = siginfo;
		pending->next = pendingSignals;
		pendingSignals = pending;
	}
	else
	{
		PendingSignal* current = pendingSignals;
		while (current->next && isMoreImportantSignalThan(
					current->next->siginfo.si_signo, siginfo.si_signo))
		{
			current = current->next;
		}
		if (unlikely(current->siginfo.si_signo == siginfo.si_signo))
		{
			return;
		}
		PendingSignal* pending = new PendingSignal;
		pending->siginfo = siginfo;
		pending->next = current->next;
		current->next = pending;
	}
	updatePendingSignals();
}

void Process::updatePendingSignals()
{
	signalPending = (pendingSignals != nullptr);
}

int Syscall::kill(pid_t pid, int signal)
{
	if (signal < 0 || signal >= NSIG)
	{
		errno = EINVAL;
		return -1;
	}
	Process* process;
	if (pid == Process::current->pid)
	{
		process = Process::current;
	}
	else
	{
		errno = EPERM;
		return -1;
	}

	if (signal == 0)
	{
		return 0;
	}
	siginfo_t siginfo = {};
	siginfo.si_signo = signal;
	siginfo.si_code = SI_USER;
	siginfo.si_pid = Process::current->pid;
	process->raiseSignal(siginfo);

	return 0;
}
