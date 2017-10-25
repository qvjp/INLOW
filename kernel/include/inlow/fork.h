#ifndef _INLOW_FORK_H
#define _INLOW_FORK_H

#include <stdint.h>

#define RFPROC (1 << 0)
#define RFFDG (1 << 1)

#define _RFFORK (RFPROC | RFFDG)

struct regfork
{
	uint32_t rf_esp;
	uint32_t rf_eip;
	uint32_t rf_eax;
	uint32_t rf_ebx;
	uint32_t rf_ecx;
	uint32_t rf_edx;
	uint32_t rf_esi;
	uint32_t rf_edi;
	uint32_t rf_ebp;
};

#endif
