#ifndef KERNEL_INITRD_H
#define KERNEL_INITRD_H

#include <inlow/kernel/directory.h>
#include <inlow/kernel/kernel.h>

namespace Initrd
{
	DirectoryVnode* loadInitrd(vaddr_t initrd);
}

#endif
