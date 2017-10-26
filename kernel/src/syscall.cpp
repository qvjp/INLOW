#include <errno.h>
#include <inlow/fcntl.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/process.h>
#include <inlow/kernel/syscall.h>

static const void* syscallList[NUM_SYSCALLS] = {
	(void*) Syscall::exit,
	(void*) Syscall::write,
	(void*) Syscall::read,
	(void*) Syscall::mmap,
	(void*) Syscall::munmap,
	(void*) Syscall::openat,
	(void*) Syscall::close,
	(void*) Syscall::regfork,
	(void*) Syscall::execve,
	(void*) Syscall::waitpid,
	(void*) Syscall::fstatat,
};

static FileDescription* getRootFd(int fd, const char* restrict path)
{
	if (path[0] == '/')
			return Process::current->rootFd;
	else if (fd == AT_FDCWD)
			return Process::current->cwdFd;
	else
			return Process::current->fd[fd];
}

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

int Syscall::close(int fd)
{
	FileDescription* descr = Process::current->fd[fd];
	if (!descr)
	{
		errno = EBADF;
		return -1;
	}

	delete descr;
	Process::current->fd[fd] = nullptr;
	return 0;
}

int Syscall::execve(const char* path, char* const argv[], char* const envp[])
{
	FileDescription* descr = Process::current->rootFd->openat(path, 0, 0);

	if (!descr || Process::current->execute(descr, argv, envp) == -1)
	{
		return -1;
	}

	// Schedule
	asm volatile ("int $0x31");
	__builtin_unreachable();
}

NORETURN void Syscall::exit(int status)
{
	Process::current->exit(status);
	asm volatile ("int $0x31");
	__builtin_unreachable();
}

int Syscall::fstatat(int fd, const char* restrict path, struct stat* restrict result, int)
{
	FileDescription* descr = getRootFd(fd, path);

	Vnode* vnode = descr->vnode->openat(path, 0, 0);
	if (!vnode)
			return -1;
	return vnode->stat(result);
}
int Syscall::openat(int fd, const char* path, int flags, mode_t mode)
{
	FileDescription* descr = getRootFd(fd, path);
	FileDescription* result = descr->openat(path, flags,mode);
	if (!result)
			return -1;
	return Process::current->registerFileDescriptor(result);
}
ssize_t Syscall::read(int fd, void* buffer, size_t size)
{
	FileDescription* descr = Process::current->fd[fd];
	return descr->read(buffer, size);
}

pid_t Syscall::regfork(int flags, struct regfork* registers)
{
	if (!((flags & RFPROC) && (flags & RFFDG)))
	{
		errno = EINVAL;
		return -1;
	}

	Process* newProcess = Process::current->regfork(flags, registers);
	return newProcess->pid;
}

pid_t Syscall::waitpid(pid_t pid, int* status, int flags)
{
	Process* process = Process::current->waitpid(pid, flags);

	if (!process)
			return -1;
	*status = process->status;
	delete process;
	return pid;
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
