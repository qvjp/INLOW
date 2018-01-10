#ifndef _INLOW_UCONTEXT_H
#define _INLOW_UCONTEXT_H

#include <inlow/sigset.h>

typedef struct
{
	void* ss_sp;
	__SIZE_TYPE__ ss_size;
	int ss_flags;
} stack_t;

#define SS_DISABLE (1 << 0)

typedef struct
{
	#ifdef __i386__
		unsigned long __eax;
		unsigned long __ebx;
		unsigned long __ecx;
		unsigned long __edx;
		unsigned long __esi;
		unsigned long __edi;
		unsigned long __ebp;
		unsigned long __eip;
		unsigned long __eflags;
		unsigned long __esp;
	#else
		#error "mcontext_t is undefined for this architecture."
	#endif
} mcontext_t;

typedef struct ucontext_t
{
	struct ucontext_t* uc_link;
	sigset_t uc_sigmask;
	stack_t uc_stack;
	mcontext_t uc_mcontext;
} ucontext_t;

#endif
