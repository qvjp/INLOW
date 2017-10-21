#include <stdarg.h>
#include <fcntl.h>

int open(const char* path, int flags, ...)
{
	mode_t mode = 0;
	if (flags == O_CREAT)
	{
		va_list ap;
		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);	
	}

	return openat(AT_FDCWD, path, flags, mode);
}
