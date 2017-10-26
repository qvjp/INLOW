#ifndef _INLOW_STAT_H
#define _INLOW_STAT_H

#include <inlow/types.h>

#define S_IFBLK 010000
#define S_IFCHR 020000
#define S_IFIFO 030000
#define S_IFREG 040000
#define S_IFDIR 050000
#define S_IFLNK 060000
#define S_IFSOCK 070000

#define S_IFMT 070000

struct stat
{
	__mode_t st_mode;
};

#endif
