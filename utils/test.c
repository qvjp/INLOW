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
 * utils/test.c
 * 测试模块
 */
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    FILE *file = fopen("hello", "r");
    char *buffer = malloc(81);

    printf("Read from file: ");
    while (fgets(buffer, 6, file))
    {
        printf("%s", buffer);
    }

    printf("\n");

    // fgets(buffer, 81, stdin);
    // printf("You wrote: %s\n", buffer);

    char *a = malloc(10);
    char *b = malloc(10);
    char *c = malloc(10);
    int num1 = 0;
    int num2 = 0;
    char op = 0;

    while (1)
    {
        for (int i = 0; i < 10; i++)
        {
            a[i] = 0;
            b[i] = 0;
            c[i] = 0;
        }
        // printf("input op: \n");
        // fgets(c, 1, stdin);

        printf("input formula: ");
        fgets(c, 10, stdin);

        for (int i = 0, j = 0, next = 0; i < 10; i++)
        {
            if (next == 0 && c[i] != '+' && c[i] != '-' && c[i] != '*' && c[i] != '/')
            {
                a[i] = c[i];
            }
            else
            {
                next = 1;
                if (!(c[i] == '+' || c[i] == '-' || c[i] == '*' || c[i] == '/'))
                {
                    b[j++] = c[i];
                }
                else
                {
                    op = c[i];
                }
                
            }
        }

        num1 = strtol(a, &buffer, 10);
        num2 = strtol(b, &buffer, 10);

        switch(op)
        {
        case '+' :
            printf("%u %c %u = %u", num1, op, num2, num1 + num2);
            break;
        case '-' :
            printf("%u %c %u = %u", num1, op, num2, num1 - num2);
            break;
        case '*' :
            printf("%u %c %u = %u", num1, op, num2, num1 * num2);
            break;
        case '/' :
            printf("%u %c %u = %u", num1, op, num2, num1 / num2);
            break;
        default:
            printf("input error");
        }
        printf("\n");
    }
    free(buffer);
    return 0;
}
