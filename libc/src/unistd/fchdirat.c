#include <unistd.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_FCHDIRAT, int, fchdirat, (int, const char*));
