#ifndef _INLOW_DIRENT_H
#define _INLOW_DIRENT_H

#include <inlow/types.h>

struct dirent
{
	__dev_t d_dev;
	__ino_t d_ino;
	__SIZE_TYPE__ d_reclen;
	__extension__ char d_name[];
};

#endif
