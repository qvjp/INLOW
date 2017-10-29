#ifndef _TERMIOS_H
#define _TERMIOS_H

#include <sys/cdefs.h>
#define __need_pid_t
#include <sys/libc-types.h>
#include <inlow/termios.h>

#ifdef __cplusplus
extern "C"
{
#endif

		int tcgetattr(int, struct termios*);
		int tcsetattr(int, int, const struct termios*);

#ifdef __cplusplus
}
#endif

#endif
