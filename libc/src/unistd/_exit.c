#include <sys/syscall.h>
#include <unistd.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_EXIT, __noreturn void, _exit, (int));
