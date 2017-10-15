#ifndef KERNEL_PORT_H
#define KERNEL_PORT_H

#include <stdint.h>
static inline uint8_t inb(uint16_t port)
{
	uint8_t result;
	asm volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
	return result;
}

static inline void outb(uint16_t port, uint8_t value)
{
	asm volatile ("outb %0, %1" :: "a"(value), "Nd"(port));
}


#endif
