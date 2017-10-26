#ifndef _INLOW_WAIT_H
#define _INLOW_WAIT_H

#define WNOHANG (1 << 0)
#define WUNTRACED (1 << 1)

#define WEXITSTATUS(status) ((status) & 0xFF)
#define WIFEXITED(status) 1

#endif
