/* Copyright (c) 2016, Dennis Wölfing
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* kernel/src/arch/i686/syscall.S
 * Syscall handler.
 */

.section .text
.global syscallHandler
.type syscallHandler, @function
syscallHandler:
    mov %esp, %ebp
    and $(~0xFF), %esp
    sub $12, %esp
    push %edi

    push %esi
    push %edx
    push %ecx
    push %ebx

    sub $12, %esp
    push %eax

    mov $0x10, %cx
    mov %cx, %ds

    call getSyscallHandler
    add $16, %esp

    call *%eax

    mov $0x23, %cx
    mov %cx, %ds

    mov %ebp, %esp
    iret
.size syscallHandler, . - syscallHandler
