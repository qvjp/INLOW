#include <unistd.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_EXECVE, int, execve, (const char*, char* const[], char* const[]));
