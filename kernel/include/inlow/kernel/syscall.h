/** MIT License
 * 
 * Copyright (c) 2019 INLOW-OS
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * kernel/include/inlow/kernel/syscall.h
 * 系统调用函数声明
 */
#ifndef KERNEL_SYSCALL_H__
#define KERNEL_SYSCALL_H__

#include <sys/types.h>
#include <inlow/syscall.h>

struct __mmapRequest;

namespace Syscall
{
    // void pad(void);
    __dead void exit(int status);
    ssize_t read(int fd, void* buffer, size_t size);
    ssize_t write(int fd, const void* buffer, size_t size);
    void* mmap(__mmapRequest* request);
    int munmap(void* addr, size_t size);
    int openat(int fd, const char* path, int flags, mode_t mode);
    void badSyscall();
}


#endif /* KERNEL_SYSCALL_H__ */
