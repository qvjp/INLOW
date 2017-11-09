#include <errno.h>
#include <string.h>
#include <inlow/conf.h>
#include <inlow/kernel/syscall.h>

#define SYSNAME "INLOW"
#ifndef INLOW_VERSION
#define INLOW_VERSION "unknow"
#endif

#ifdef __i386__
#define MACHINE "i686"
#else
#error "Unkone architecture."
#endif

static const char* getConfstr(int name)
{
	switch (name)
	{
		case _CS_UNAME_SYSNAME:
				return SYSNAME;
		case _CS_UNAME_RELEASE:
				return INLOW_VERSION;
		case _CS_UNAME_VERSION:
				return __DATE__;
		case _CS_UNAME_MACHINE:
				return MACHINE;
		default:
				return NULL;
	}
}

size_t Syscall::confstr(int name, char* buffer, size_t size)
{
	const char* result = getConfstr(name);
	if (!result)
	{
		errno = EINVAL;
		return 0;
	}
	return strlcpy(buffer, result, size);
}
