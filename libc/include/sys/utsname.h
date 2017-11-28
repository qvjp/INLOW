#ifndef _SYS_UTSNAME_H
#define _SYS_UTSNAME_H

#include <sys/cdefs.h>
#ifdef __cplusplus
extern "C"
{
#endif

#define _UNAME_LENGTH 65
#define _NODENAME_LENGTH 256

		struct utsname
		{
			char sysname[_UNAME_LENGTH];
			char nodename[_NODENAME_LENGTH];
			char release[_UNAME_LENGTH];
			char version[_UNAME_LENGTH];
			char machine[_UNAME_LENGTH];
		};

		int uname(struct utsname*);
#ifdef _cplusplus
}
#endif

#endif
