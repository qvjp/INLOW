#ifndef _DIRENT_H
#define _DIRENT_H

#include <sys/cdefs.h>
#include <inlow/dirent.h>

#ifdef __cplusplue
extern "C"
{
#endif
		typedef struct __DIR DIR;

#ifdef __is_inlow_libc
		struct __DIR {
				int fd;
				struct dirent* dirent;
				unsigned long offset;
		};
#endif
		int closedir(DIR*);
		DIR* fdopendir(int);
		DIR* opendir(const char*);
		struct dirent* readdir(DIR*);
		void rewinddir(DIR*);
#ifdef __cplusplus
}
#endif

#endif
