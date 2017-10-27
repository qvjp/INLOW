#ifndef _INLOW_DIRENT_H
#define _INLOW_DIRENT_H

struct dirent
{
	__SIZE_TYPE__ d_reclen;
	__extension__ char d_name[];
};

#endif
