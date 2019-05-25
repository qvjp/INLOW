#ifndef SYS_TYPES_H_
#define SYS_TYPES_H_

#include <sys/cdefs.h>

#define __need_blkcnt_t
#define __need_blksize_t
#define __need_clock_t
#define __need_clockid_t
#define __need_dev_t
#define __need_fsblkcnt_t
#define __need_fsfilcnt_t
#define __need_gid_t
#define __need_id_t
#define __need_ino_t
#define __need_mode_t
#define __need_nlink_t
#define __need_off_t
#define __need_pid_t
/* pthreads ... */
#define __need_size_t
#define __need_ssize_t
#define __need_time_t
#define __need_timer_t
#define __need_uid_t
#include <sys/libc-types.h>

#endif
