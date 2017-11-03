#include <errno.h>
#include <sys/stat.h>
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
	(void*) Syscall::readdir,
	(void*) Syscall::nanosleep,
	(void*) Syscall::tcgetattr,
	(void*) Syscall::tcsetattr,
	(void*) Syscall::fchdirat,
};

static FileDescription* getRootFd(int fd, const char* path)
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
	FileDescription* descr = getRootFd(AT_FDCWD, path)->openat(path, 0, 0);

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

int Syscall::fchdirat(int fd, const char* path)
{
	FileDescription* descr = getRootFd(fd, path);
	FileDescription* newCwd = descr->openat(path, 0, 0);
	if (!newCwd)
			return -1;
	if (!S_ISDIR(newCwd->vnode->mode))
	{
		errno = ENOTDIR;
		return -1;
	}

	delete Process::current->cwdFd;
	Process::current->cwdFd = newCwd;
	return 0;
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

ssize_t Syscall::readdir(int fd, unsigned long offset, void* buffer, size_t size)
{
	FileDescription* descr = Process::current->fd[fd];
	return descr->readdir(offset, buffer, size);
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

int Syscall::tcgetattr(int fd, struct termios* result)
{
	FileDescription * descr = Process::current->fd[fd];
	return descr->tcgetattr(result);
}

int Syscall::tcsetattr(int fd, int flags, const struct termios* termio)
{
	FileDescription* descr = Process::current->fd[fd];
	return descr->tcsetattr(flags, termio);
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
