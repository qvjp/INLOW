#include <inlow/kernel/print.h>
#include <inlow/kernel/process.h>
#include <inlow/kernel/syscall.h>

static const void* syscallList[NUM_SYSCALLS] = {
	(void*) Syscall::exit,
	(void*) Syscall::write,
	(void*) Syscall::read,
};

extern "C" const void* getSyscallHandler(unsigned interruptNumber)
{
	if (interruptNumber >= NUM_SYSCALLS)
	{
		return (void*) Syscall::badSyscall;
	}
	else
	{
		return syscallList[interruptNumber];
	}
}

NORETURN void Syscall::exit(int status)
{
	Process::current->exit(status);
	asm volatile ("int $0x31");
	__builtin_unreachable();
}

ssize_t Syscall::read(int fd, void* buffer, size_t size)
{
	FileDescription* descr = Process::current->fd[fd];
	return descr->read(buffer, size);
}

ssize_t Syscall::write(int fd, const void* buffer, size_t size)
{
	FileDescription* descr = Process::current->fd[fd];
	return descr->write(buffer, size);
}

void Syscall::badSyscall()
{
	Print::printf("Syscall::badSyacall was called!\n");
}
