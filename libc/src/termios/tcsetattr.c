#include <termios.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_TCSETATTR, int, tcsetattr, (int, int, const struct termios*));
