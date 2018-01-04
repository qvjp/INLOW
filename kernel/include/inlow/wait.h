#ifndef _INLOW_WAIT_H
#define _INLOW_WAIT_H

#define WNOHANG (1 << 0)
#define WUNTRACED (1 << 1)

#define _WEXITED 0
#define _WSIGNALED 1
#define _WSTATUS(reason, si_status) ((reason) << 24 | ((si_status) & 0xFF))

#define WEXITSTATUS(status) ((status) & 0xFF)
#define WIFEXITED(status) (((status) >> 24 & 0xFF) == _WEXITED)
#define WIFSIGNALED(status) (((status) >> 24 & 0xFF) == _WSIGNALED)
#define WTERMSIG(status) ((status) & 0xFF)

#endif
