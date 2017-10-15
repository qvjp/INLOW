#include <stdarg.h>
#include <stddef.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/terminal.h>


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
		terminal.write(&c, 1);
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
