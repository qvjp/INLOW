#include <sys/mman.h>
#include <sys/syscall.h>

DEFINE_SYSCALL(SYSCALL_MMAP, void*, sys_mmap, (struct __mmapRequest*));
void* mmap(void* addr, size_t size, int protection, int flags, int fd, off_t offset)
{
	struct __mmapRequest request = {
		._addr = addr,
		._size = size,
		._protection = protection,
		._flags = flags,
		._fd = fd,
		._offset = offset,
	};

	return sys_mmap(&request);
}
