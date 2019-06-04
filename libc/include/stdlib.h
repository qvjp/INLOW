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
 * lib/include/stdlib.h
 * 标准库定义
 */
#ifndef STDLIB_H_
#define STDLIB_H_

#include <sys/cdefs.h>
#define __need_NULL
#define __need_size_t
#include <sys/libc-types.h>
// #include <stddef.h> /* size_t NULL */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

__dead void _Exit(int);
__dead void exit(int);

void free(void*);
void* malloc(size_t);
long strtol(const char* __restrict, char** __restrict, int);
unsigned long strtoul(const char* __restrict, char** __restrict, int);


__dead void abort(void);

int atexit(void (*)(void));
int atio(const char*);
char* getenv(const char*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STDLIB_H__ */
