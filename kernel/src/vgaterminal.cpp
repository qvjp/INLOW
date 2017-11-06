#include <string.h>
#include <inlow/kernel/kernel.h>
#include <inlow/kernel/port.h>
#include <inlow/kernel/vgaterminal.h>

#define HEIGHT 25
#define WIDTH 80
#define VIDEO_SIZE 2 * WIDTH * HEIGHT
#define MAX_PARAMS 16

static char* const video = (char*) 0xC0000000;
static uint8_t color = 0x07; //gray on black
static unsigned int cursorPosX = 0;
static unsigned int cursorPosY = 0;

static unsigned int savedX = 0;
static unsigned int savedY = 0;

static unsigned int params[MAX_PARAMS];
static bool paramSpecified[MAX_PARAMS];
static size_t paramIndex;

static enum
{
	NORMAL,
	ESCAPED,
	CSI,
} status = NORMAL;

static inline char* videoOffset(unsigned int line, unsigned int column)
{
	return video + 2 * line * WIDTH + 2 * column;
}

static void clear(char* offset, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		offset[i] = 0;
		offset[i + 1] = color;
	}
}

void VgaTerminal::backspace()
{
	if (cursorPosX == 0 && cursorPosY > 0)
	{
		cursorPosX = WIDTH - 1;
		cursorPosY--;
	}
	else
	{
		cursorPosX--;
	}
	*videoOffset(cursorPosY, cursorPosX) = 0;
	*(videoOffset(cursorPosY, cursorPosX) + 1) = 0x07;
}

static void setGraphicsRendition()
{
	for (size_t i = 0; i < MAX_PARAMS; i++)
	{
		if (!paramSpecified[i])
			continue;
		switch (params[i])
		{
			case 0: // Reset
				color = 0x07;
				break;
			case 1: // Increased intensity
				color |= 0x08;
				break;
			case 22: // Normal intensity
				color &= ~0x08;
				break;
			case 30:
				color = (color & 0xF8) | 0x00;
				break;
			case 31:
				color = (color & 0xF8) | 0x04;
				break;
			case 32:
				color = (color & 0xF8) | 0x02;
				break;
			case 33:
				color = (color & 0xF8) | 0x06;
				break;
			case 34:
				color = (color & 0xF8) | 0x01;
				break;
			case 35:
				color = (color & 0xF8) | 0x05;
				break;
			case 36:
				color = (color & 0xF8) | 0x03;
				break;
			case 37:
			case 39:
				color = (color & 0xF8) | 0x07;
				break;
			case 40:
			case 49:
				color = (color & 0x0F) | 0x00;
				break;
			case 41:
				color = (color & 0x0F) | 0x40;
				break;
			case 42:
				color = (color & 0x0F) | 0x20;
				break;
			case 43:
				color = (color & 0x0F) | 0x60;
				break;
			case 44:
				color = (color & 0x0F) | 0x10;
				break;
			case 45:
				color = (color & 0x0F) | 0x50;
				break;
			case 46:
				color = (color & 0x0F) | 0x30;
				break;
			case 47:
				color = (color & 0x0F) | 0x70;
				break;

			default:
				break;
		}
	}
}

void VgaTerminal::printCharacter(char c)
{
	if (likely(status == NORMAL && c != '\e'))
	{
		printCharacterRaw(c);
		return;
	}

	if (status == NORMAL)
	{
		status = ESCAPED;
	}
	else if (status == ESCAPED)
	{
		if (c == '[')
		{
			status = CSI;
			for (size_t i = 0; i < MAX_PARAMS; i++)
			{
				params[i] = 0;
				paramSpecified[i] = false;
			}
			paramIndex = 0;
		}
		else if (c == 'c')
		{
			color = 0x07;
			clear(video, VIDEO_SIZE);
			cursorPosX = cursorPosY = 0;
			savedX = savedY = 0;
			status = NORMAL;
		}
		else
		{
			status = NORMAL;
		}
	}
	else if (status == CSI)
	{
		if (c >= '0' && c <= '9')
		{
			params[paramIndex] = params[paramIndex] * 10 + c -'0';
			paramSpecified[paramIndex] = true;
		}
		else if (c == ';')
		{
			paramIndex++;
			if (paramIndex >= MAX_PARAMS)
			{
				status = NORMAL;
			}
		}
		else
		{
			switch (c)
			{
				case 'A':
					{
						unsigned int param = paramSpecified[0] ? params[0] : 1;
						if (cursorPosY < param)
						{
							cursorPosY = 0;
						}
						else
						{
							cursorPosY -= param;
						}
					}
					break;
				case 'B':
					{
						unsigned int param = paramSpecified[0] ? params[0] : 1;
						if (cursorPosY + param >= HEIGHT)
						{
							cursorPosY = HEIGHT - 1;
						}	
						else
						{
							cursorPosY += param;
						}
					}
					break;
				case 'C':
					{
						unsigned int param = paramSpecified[0] ? params[0] : 1;
						if (cursorPosX + param >= WIDTH)
						{
							cursorPosX = 0;
						}
						else
						{
							cursorPosX += param;
						}
					}
					break;
				case 'D':
					{
						unsigned int param = paramSpecified[0] ? params[0] : 1;
						if (cursorPosX < param)
						{
							cursorPosX = 0;
						}
						else
						{
							cursorPosX -= param;
						}
					}
					break;
				case 'E':
					{
						unsigned int param = paramSpecified[0] ? params[0] : 1;
						if (cursorPosY + param >= HEIGHT)
						{
							cursorPosY = HEIGHT - 1;
						}
						else
						{
							cursorPosY += param;
						}
						cursorPosX = 0;
					}
					break;
				case 'F':
					{
						unsigned int param = paramSpecified[0] ? params[0] : 1;
						if (cursorPosY < param)
						{
							cursorPosY = 0;
						}
						else
						{
							cursorPosY -= param;
						}
						cursorPosX = 0;
					}
					break;
				case 'G':
					{
						unsigned int param = paramSpecified[0] ? params[0] : 1;
						if (0 < param && param < WIDTH)
						{
							cursorPosX = param - 1;
						}
					}
					break;
				case 'H':
				case 'f':
					{
						unsigned int x = paramSpecified[1] ? params[1] : 1;
						unsigned int y = paramSpecified[0] ? params[0] : 1;
						if (0 < x && x < WIDTH && 0 < y && y < HEIGHT)
						{
							cursorPosX = x - 1;
							cursorPosY = y - 1;
						}
					}
					break;
				case 'J': 
					{
						unsigned int param = paramSpecified[0] ? params[0] : 0;
						if (param == 0) 
						{
							char* from = videoOffset(cursorPosY, cursorPosX);
							clear(from, VIDEO_SIZE - (from - video));
						} else if (param == 1) 
						{
							char* to = videoOffset(cursorPosY, cursorPosX);
							clear(video, to - video);
						} else if (param == 2) 
						{
							clear(video, VIDEO_SIZE);
						}
					} break;
				case 'K': 
					{
						unsigned int param = paramSpecified[0] ? params[0] : 0;
						if (param == 0) 
						{
							char* from = videoOffset(cursorPosY, cursorPosX);
							clear(from, videoOffset(cursorPosY + 1, 0) - from);
						} else if (param == 1) 
						{
							char* from = videoOffset(cursorPosY, 0);
							char* to = videoOffset(cursorPosY, cursorPosX);
							clear(from, to - from);
						} else if (param == 2) 
						{
							clear(videoOffset(cursorPosY, 0), 2 * WIDTH);
						}
					} 
					break;
				case 'S':
					{
						unsigned int param = paramSpecified[0] ? params[0] : 1;
						memmove(video, videoOffset(param, 0),
								2 * (HEIGHT - param) * WIDTH);
						clear(videoOffset(HEIGHT - param, 0), 2 * param * WIDTH);
					} 
					break;
				case 'T': {
							  unsigned int param = paramSpecified[0] ? params[0] : 1;
							  memmove(videoOffset(param, 0), video,
									  2 * (HEIGHT - param) * WIDTH);
							  clear(video, 2 * WIDTH * param);
						  } 
						  break;
				case 'd': 
						  { 
							  unsigned int param = paramSpecified[0] ? params[0] : 1;
							  if (0 < param && param < HEIGHT) 
							  {
								  cursorPosY = param - 1;
							  }
						  } 
						  break;
				case 'm': 
						  {
							  setGraphicsRendition();
						  }
						  break;
				case 's': 
						  {
							  savedX = cursorPosX;
							  savedY = cursorPosY;
						  }
						  break;
				case 'u':
						  { 
							  cursorPosX = savedX;
							  cursorPosY = savedY;
						  } 
						  break;
				default:
						  // Unknown command, ignore
						  // TODO: Implement more escape sequences when needed
						  break;
			}
			status = NORMAL;
		}
	}
}

void VgaTerminal::printCharacterRaw(char c)
{
	if (c == '\n' || cursorPosX > WIDTH - 1)
	{
		cursorPosX = 0;
		cursorPosY++;

		if (cursorPosY > HEIGHT - 1)
		{
			memmove(video, videoOffset(1, 0), 2 * (HEIGHT - 1) * WIDTH);
			clear(videoOffset(HEIGHT - 1, 0), 2 * WIDTH);
			cursorPosY = HEIGHT - 1;
		}
		if (c == '\n')
			return;
	}
	*videoOffset(cursorPosY, cursorPosX) = c;
	*(videoOffset(cursorPosY, cursorPosX) + 1) = color;

	cursorPosX++;
}
void VgaTerminal::updateCursorPosition()
{
	uint16_t position = cursorPosX + cursorPosY * 80;
	outb(0x3D4, 0x0E);
	outb(0x3D5, (position >> 8) & 0xFF);
	outb(0x3D4, 0x0F);
	outb(0x3D5, position & 0xFF);
}
