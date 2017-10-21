#include <stdarg.h>
#include <fcntl.h>
#include <sys/syscall.h>

DEFINE_SYSCALL(SYSCALL_OPENAT, int, sys_openat, (int, const char*, int, mode_t));

int openat(int fd, const char* path, int flags, ...)
{
	mode_t mode = 0;
	if (flags & O_CREAT)
	{
		va_list ap;
		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);
	}
	return sys_openat(fd, path, flags, mode);
}
