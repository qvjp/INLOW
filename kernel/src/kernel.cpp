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
 * kernel/src/kernel.cpp
 * 内核main函数
 */

#include <stddef.h> /* size_t */
#include <stdint.h> /* uint8_t */
#include <inlow/kernel/print.h> /* printf() */

extern "C" void kernel_main()
{
    Print::initTerminal();
    Print::printf("HELLO WORLD!\n");
    Print::printf("I'm INLOW!\n");
    Print::printf("I'm %d years old!\n", 2);
    Print::printf("unsigned int max %u\n", 4294967295);
    Print::printf("unsigned int min %u\n", 0);
    Print::printf("signed int max %d\n", 2147483647);
    Print::printf("signed int min %d\n", -2147483648);
    Print::printf("hex number 0x%x\n", 4294967295);
}
