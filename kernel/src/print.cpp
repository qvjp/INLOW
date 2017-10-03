#include <stdarg.h>
#include <stddef.h>
#include <inlow/kernel/print.h>

static char* video = (char*) 0xC0000000;
static int cursorPostX = 0;
static int cursorPostY = 0;

static void printChar(char c);
static void printString(const char* s);
static void printNum(unsigned u, int base);

void Print::printf(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

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
				case 'u':
						u = va_arg(ap, unsigned);
						printNum(u, 10);
						break;
				case 'x':
						u = va_arg(ap, unsigned);
						printNum(u, 16);
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

static void printChar(char c)
{
	if (c == '\n' || cursorPostX > 79)
	{
		cursorPostX = 0;
		cursorPostY++;
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

static void printString(const char* s)
{
	while (*s)
	{
		printChar(*s++);
	}
}

static void printNum(unsigned u, int base)
{
	static const char* digits = "0123456789abcdef";

	char buffer[11];
	char* p = buffer + 10;
	*p = '\0';

	do {
		*--p = digits[u % base];
		u /= base;
	} while (u);

	printString(p);
}
