#include <errno.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <inlow/fcntl.h>
#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/process.h>
#include <inlow/kernel/symlink.h>
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
	(void*) Syscall::confstr,
	(void*) Syscall::fstat,
	(void*) Syscall::mkdirat,
	(void*) Syscall::unlinkat,
	(void*) Syscall::renameat,
	(void*) Syscall::linkat,
	(void*) Syscall::symlinkat,
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

static Reference<Vnode> resolvePathExceptLastComponent(int fd, char* path, char** lastComponent)
{
	Reference<Vnode> vnode = getRootFd(fd, path)->vnode;

	return resolvePathExceptLastComponent(vnode, path, lastComponent);
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
	FileDescription* descr = getRootFd(AT_FDCWD, path);
	Reference<Vnode> vnode = resolvePath(descr->vnode, path);

	if (!vnode || Process::current->execute(vnode, argv, envp) == -1)
	{
		return -1;
	}

	sched_yield();
	__builtin_unreachable();
}

NORETURN void Syscall::exit(int status)
{
	Process::current->exit(status);
	sched_yield();
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

int Syscall::fstat(int fd, struct stat* result)
{
	FileDescription* descr = Process::current->fd[fd];
	return descr->vnode->stat(result);
}

int Syscall::fstatat(int fd, const char* restrict path, struct stat* restrict result, int flags)
{
	bool followFinalSymlink = !(flags & AT_SYMLINK_NOFOLLOW);
	FileDescription* descr = getRootFd(fd, path);

	Reference<Vnode> vnode = resolvePath(descr->vnode, path, followFinalSymlink);
	if (!vnode)
			return -1;
	return vnode->stat(result);
}

int Syscall::linkat(int oldFd, const char* oldPath, int newFd, const char* newPath, int flags)
{
	bool followFinalSymlink = flags & AT_SYMLINK_FOLLOW;
	Reference<Vnode> vnode = resolvePath(getRootFd(oldFd, oldPath)->vnode, oldPath, followFinalSymlink);
	if (!vnode)
		return -1;
	if (S_ISDIR(vnode->mode))
	{
		errno = EPERM;
		return -1;
	}

	char* name;
	char* path2Copy = strdup(newPath);
	if (!path2Copy)
		return -1;
	Reference<Vnode> directory = resolvePathExceptLastComponent(newFd, path2Copy, &name);
	if (!directory)
	{
		free(path2Copy);
		return -1;
	}

	int result = directory->link(name, vnode);
	free(path2Copy);
	return result;
}

static void* mmapImplementation(void*, size_t size, int protection, int flags, int, off_t)
{
	if (size == 0 || !(flags & MAP_PRIVATE))
	{
		errno = EINVAL;
		return MAP_FAILED;
	}
	if (flags & MAP_ANONYMOUS)
	{
		AddressSpace* addressSpace = Process::current->addressSpace;
		return (void*) addressSpace->mapMemory(size, protection);
	}
	errno = ENOTSUP;
	return MAP_FAILED;
}

int Syscall::mkdirat(int fd, const char* path, mode_t mode)
{
	char* pathCopy = strdup(path);
	if (!pathCopy)
		return -1;

	char* name;
	Reference<Vnode> vnode = resolvePathExceptLastComponent(fd, pathCopy, &name);
	if (!vnode)
	{
		free(pathCopy);
		return -1;
	}

	if (!*name)
	{
		free(pathCopy);
		errno = EEXIST;
		return -1;
	}

	int result = vnode->mkdir(name, mode & ~Process::current->umask);
	free(pathCopy);
	return result;
}

void* Syscall::mmap(__mmapRequest* request)
{
	return mmapImplementation(request->_addr, 
					request->_size, 
					request->_protection, 
					request->_flags,
				   	request->_fd,
				   	request->_offset);
}

int Syscall::munmap(void* addr, size_t size)
{
	if (size == 0 || ((vaddr_t) addr & 0xFFF))
	{
		errno = EINVAL;
		return -1;
	}
	AddressSpace* addressSpace = Process::current->addressSpace;
	addressSpace->unmapMemory((vaddr_t) addr, size);
	return 0;
}

int Syscall::openat(int fd, const char* path, int flags, mode_t mode)
{
	FileDescription* descr = getRootFd(fd, path);
	FileDescription* result = descr->openat(path, flags,mode & ~Process::current->umask);
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

int Syscall::renameat(int oldFd, const char* oldPath, int newFd, const char* newPath)
{
	char* oldCopy = strdup(oldPath);
	if (!oldPath)
		return -1;
	char* oldName;
	Reference<Vnode> oldDirectory = resolvePathExceptLastComponent(oldFd, oldCopy, &oldName);
	if (!oldDirectory)
	{
		free(oldCopy);
		return -1;
	}

	char* newCopy = strdup(newPath);
	if (!newCopy)
	{
		free(oldCopy);
		return -1;
	}

	char* newName;
	Reference<Vnode> newDirectory = resolvePathExceptLastComponent(newFd, newCopy, &newName);
	if (!newDirectory)
	{
		free(oldCopy);
		free(newCopy);
		return -1;
	}

	if (strcmp(oldName, ".") == 0 || strcmp(oldName, "..") == 0 ||
					strcmp(newName, ".") == 0 || strcmp(newName, "..") == 0)
	{
		free(oldCopy);
		free(newCopy);
		errno = EINVAL;
		return -1;
	}

	int result = newDirectory->rename(oldDirectory, oldName, newName);
	free(oldCopy);
	free(newCopy);
	return result;
}

int Syscall::symlinkat(const char* targetPath, int fd, const char* linkPath)
{
	if (!*targetPath)
	{
		errno = ENOENT;
		return -1;
	}

	char* pathCopy = strdup(linkPath);
	if (!pathCopy)
		return -1;

	char* name;
	Reference<Vnode> vnode = resolvePathExceptLastComponent(fd, pathCopy, &name);
	if (!vnode)
	{
		free(pathCopy);
		return -1;
	}
	Reference<Vnode> symlink(new SymlinkVnode(targetPath, vnode->dev, 0));
	int result = vnode->link(name, symlink);
	free(pathCopy);
	return result;
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

int Syscall::unlinkat(int fd, const char* path, int flags)
{
	if (!(flags & (AT_REMOVEDIR | AT_REMOVEFILE)))
	{
		flags |= AT_REMOVEFILE;
	}

	if (path[strlen(path) - 1] == '/')
	{
		flags &= ~AT_REMOVEFILE;
	}

	char* pathCopy = strdup(path);
	if (!pathCopy)
		return -1;

	char* name;
	Reference<Vnode> vnode = resolvePathExceptLastComponent(fd, pathCopy, &name);
	if (!vnode)
	{
		free(pathCopy);
		return -1;
	}

	if (unlikely(!*name && vnode == Process::current->rootFd->vnode))
	{
		errno = EBUSY;
		return -1;
	}

	if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
	{
		free(pathCopy);
		errno = EINVAL;
		return -1;
	}
	int result = vnode->unlink(name, flags);
	free(pathCopy);
	return result;
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
