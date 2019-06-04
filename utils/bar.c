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
 * utils/bar.s
 * 测试模块，直接退出，状态码73
 */
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    (void) argc; (void) argv;
    // printf("Hello %s from userspace!\n", "xiaoming");

    FILE* file = fopen("hello", "r");
    char* buffer = malloc(81);

    while (fgets(buffer, 20, file)) {
        printf("Read from file: %s\n", buffer);
    }

    // fgets(buffer, 81, stdin);
    // printf("You wrote: %s\n", buffer);

    free(buffer);
    return 42;
}
