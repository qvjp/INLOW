#ifndef _SYS_SYSCALL_H
#define _SYS_SYSCALL_H

#include <inlow/syscall.h>

#define _SYSCALL_TOSTRING(x) #x

#define _SYSCALL_BODY(number, name) \
						asm("\n" \
						".pushsection .text\n" \
						#name	":\n\t" \
						"mov $" _SYSCALL_TOSTRING(number) ", %eax\n\t" \
						"jmp __syscall\n" \
						".popsection\n")

#define DEFINE_SYSCALL(number, type, name, params) \
								_SYSCALL_BODY(number, name); \
								extern type name params

#define DEFINE_SYSCALL_GLOBAL(number, type, name, params) \
								asm(".global " #name); \
								DEFINE_SYSCALL(number, type, name, params)

#endif
