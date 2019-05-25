#include <sys/mman.h>
#include <sys/syscall.h>

DEFINE_SYSCALL_GLOBAL(SYSCALL_MUNMAP, int, munmap, (void*, size_t));