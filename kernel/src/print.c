#include <stdarg.h>                /* va_list va_start() va_arg() va_end() */
#include <stddef.h>                /* size_t */
#include <inlow/kernel/print.h>

// 在_start中视频内存首地址被映射到0xC0000000
static char* video = (char*) 0xC0000000;
static int cursorPostX = 0;
static int cursorPostY = 0;

static void printChar(char c);
static void printString(const char* s);
static void printUNum(unsigned u, int base);
static void printSNum(int u, int base);

void printf(const char* format, ...)
{
    /*
     * 在这里写一下C语言中可变参数的基本用法
     * 
     * 其中va是指variable-argument(可变参数)
     * va_list是一个arg_ptr,指向参数的指针
     * va_start()初始化va_list对象
     * va_arg()返回参数指针当前指向的值
     * va_end()和va_start()对应，调用过va_start后就必须调用va_end
     * 
     * */
    va_list ap;
    va_start(ap, format);

    int d;
    unsigned u;
    unsigned char c;
    const char* s;
    while (*format)
    {
        if(*format != '%')
        {
            printChar(*format);
        }
        else
        {
            switch(*++format)
            {
                case 'd':
                        d = va_arg(ap, int);
                        printSNum(d, 10);
                        break;
                case 'u':
                        u = va_arg(ap, unsigned);
                        printUNum(u, 10);
                        break;
                case 'x':
                        u = va_arg(ap, unsigned);
                        printUNum(u, 16);
                        break;
                case 'c':
                        c = (unsigned char) va_arg(ap, int);
                        printChar(c);
                        break;
                case 's':
                        s = va_arg(ap, char*);
                        printString(s);
                        break;
                case '%':
                        printChar('%');
                        break;
                case '\0':
                        va_end(ap);
                        return;
                default:
                        printChar('%');
                        printChar(*format);
            }
        }
        format++;
    }
    va_end(ap);
}

// 单个字符
static void printChar(char c)
{
    // 遇到换行符或超出最大列数(79)就换行
    if (c == '\n' || cursorPostX > 79)
    {
        cursorPostX = 0;
        cursorPostY++;
        // 如果当前行是最后一行，则整体向上移动一行
        if(cursorPostY > 24)
        {
            for(size_t i = 0; i < 2 * 24 * 80; i++)
            {
                video[i] = video[i +2 * 80];
            }
            for (size_t i = 2 * 24 * 80; i < 2 * 25 * 80; i++)
            {
                video[i] = 0;
            }
            cursorPostY = 24;
        }
        if (c == '\n')
                return;
    }
    video[cursorPostY * 2 * 80 + 2 * cursorPostX] = c;
    video[cursorPostY * 2 * 80 + 2 * cursorPostX + 1] = 0x07;

    cursorPostX++;
}

// 字符串
static void printString(const char* s)
{
    while (*s)
    {
        printChar(*s++);
    }
}

// 无符号正数
static void printUNum(unsigned u, int base)
{
    static const char* digits = "0123456789abcdef";

    // 因为32位整数最大为4,294,967,296，所以buffer选了11位
    char buffer[11];
    char* p = buffer + 10;
    *p = '\0';

    do {
        *--p = digits[u % base];
        u /= base;
    } while (u);

    printString(p);
}


// 有符号正数
/* !BUG 
 最小为-2147483647而非标准的-2147483648
*/
static void printSNum(int u, int base)
{
    int flag = 0; //正
    if (u < 0)
    {
        u = -u;
        flag = 1;
    }
    static const char* digits = "0123456789abcdef";

    char buffer[12];
    char* p = buffer + 11;
    *p = '\0';

    do {
        *--p = digits[u % base];
        u /= base;
    } while (u);

    if (!flag)
        printString(p);
    else
    {
        *--p = '-';
        printString(p);        
    }
}
