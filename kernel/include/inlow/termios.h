#ifndef _INLOW_TERMIOS_H
#define _INLOW_TERMIOS_H

#define ECHO (1 << 0)
#define ICANON (1 << 1)

#define VEOF 0
#define VEOL 1
#define VERASE 2
#define VINTR 3
#define VKILL 4
#define VMIN 5
#define VQUIT 6
#define VSTART 7
#define VSTOP 8
#define VSUSP 9
#define VTIME 10
#define NCCS 11

#define TCSAFLUSH 0

typedef unsigned char cc_t;
typedef unsigned int tcflag_t;

struct termios
{
	tcflag_t c_iflag;
	tcflag_t c_oflag;
	tcflag_t c_cflag;
	tcflag_t c_lflag;
	cc_t c_cc[NCCS];
};

#endif
