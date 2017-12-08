#ifndef _INLOW_FCNTL_H
#define _INLOW_FCNTL_H

#define AT_FDCWD (-1)

#define AT_SYMLINK_FOLLOW (1 << 0)

#define AT_REMOVEDIR (1 << 0)
#define AT_REMOVEFILE (1 << 1)

#define O_EXEC (1 << 0)
#define O_RDONLY (1 << 1)
#define O_WRONLY (1 << 2)
#define O_RDWR (O_RDONLY | O_WRONLY)
#define O_SEARCH O_EXEC

#define O_APPEND (1 << 3)
#define O_CLOEXEC (1 << 4)
#define O_CREAT (1 << 5)
#define O_DIRECTORY (1 << 6)
#define O_EXCL (1 << 7)
#define O_NOCTTY (1 << 8)
#define O_NOFOLLOW (1 << 9)
#define O_NONBLOCK (1 << 10)
#define O_SYNC (1 << 11)
#define O_TRUNC (1 << 12)
#define O_TTY_INIT (1 << 13)

#endif
