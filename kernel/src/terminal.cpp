#include <inlow/kernel/terminal.h>
Terminal terminal;

static char* video = (char*) 0xC0000000;
static int cursorPosX = 0;
static int cursorPosY = 0;

static void printCharacter(char c);

ssize_t Terminal::write(const void* buffer, size_t size)
{
	const char* buf = (const char*) buffer;

	for (size_t i = 0; i < size; i++)
	{
		printCharacter(buf[i]);
	}
	return (ssize_t) size;
}
static void printCharacter(char c)
{
	if (c == '\n' || cursorPosX > 79)
	{
		cursorPosX = 0;
		cursorPosY++;

		if (cursorPosY > 24)
		{
			for (size_t i = 0; i < 2 * 24 * 80; i++)
			{
				video[i] = video[i + 2 * 80];
			}
			for (size_t i = 2 * 24 * 80; i < 2* 25 * 80; i++)
			{
				video[i]  = 0;
			}
			cursorPosY = 24;
		}
		if (c == '\n')
				return;
	}
	video[cursorPosY * 2 * 80 + 2 * cursorPosX] = c;
	video[cursorPosY * 2 * 80 + 2 * cursorPosX + 1] = 0x07;

	cursorPosX++;
}
