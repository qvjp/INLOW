#ifndef _SYS_STAT_H
#define _SYS_STAT_H

#include <sys/cdefs.h>
#define __need_dev_t
#define __need_ino_t
#define __need_mode_t
#define __need_nlink_t
#define __need_uid_t
#define __need_gui_t
#define __need_off_t
#define __need_time_t
#include <sys/libc-types.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define S_IRWXU 0700
#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100
#define S_IRWXG 070
#define S_IRGRP 040
#define S_IWGRP 020
#define S_IXGRP 010
#define S_IRWXO 07
#define S_IROTH 04
#define S_IWOTH 02
#define S_IXOTH 01
#define S_ISUID 04000
#define S_ISGID 02000

#ifdef __cplusplus
}
#endif
#endif
