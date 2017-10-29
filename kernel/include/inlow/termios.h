#ifndef _INLOW_TERMIOS_H
#define _INLOW_TERMIOS_H

#define ECHO (1 << 0)
#define ICANON (1 << 1)

#define VMIN 0
#define NCCS 1

#define TCSAFLUSH 0

typedef unsigned char cc_t;
typedef unsigned int tcflag_t;

struct termios
{
	tcflag_t c_lflag;
	cc_t c_cc[NCCS];
};

#endif
