#ifndef KERNEL_INITRD_H__
#define KERNEL_INITRD_H__

#include <inlow/kernel/directory.h>
#include <inlow/kernel/inlow.h>

namespace Initrd {
DirectoryVnode* loadInitrd(inlow_vir_addr_t initrd);
}

#endif