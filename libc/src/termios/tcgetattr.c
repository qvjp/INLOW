#include <termios.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_TCGETATTR, int, tcgetattr, (int, struct termios*));
