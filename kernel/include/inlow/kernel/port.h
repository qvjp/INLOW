#ifndef KERNEL_PORT_H__
#define KERNEL_PORT_H__

#include <stdint.h> /* uint16_t uint8_t */

/**
 * x86处理器有两套独立的地址空间---内存地址空间和I/O地址空间
 * I/O地址空间的读写用到汇编OUT/IN
 * 发送8/16/32位的值到I/O地址，分别用outb/outw/outl
 */
static inline void outportb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %0, %1" :: "a"(value), "Nd"(port));
}

#endif /* KERNEL_PORT_H__ */
