#include <stddef.h>
#include <inlow/kernel/keyboard.h>

static const char KBLAYOUT_US[] = {
	'`', '~', 0, 0,
	0, 0, 0, 0, //Escape
	'1', '!', 0, 0,
	'2', '@', 0, 0,
	'3', '#', 0, 0,
	'4', '$', 0, 0,
	'5', '%', 0, 0,
	'6', '^', 0, 0,
	'7', '&', 0, 0,
	'8', '*', 0, 0,
	'9', '(', 0, 0,
	'0', ')', 0, 0,
	'-', '_', 0, 0,
	'=', '+', 0, 0,
	'\b', '\b', '\b', '\b',
	' ', ' ', '\t', '\t',
	'q', 'Q', 0, 0,
	'w', 'W', 0, 0,
	'e', 'E', 0, 0,
	'r', 'R', 0, 0,
	't', 'T', 0, 0,
	'y', 'Y', 0, 0,
	'u', 'U', 0, 0,
	'i', 'I', 0, 0,
	'o', 'O', 0, 0,
	'p', 'P', 0, 0,
	'[', '{', 0, 0,//0x1A
	']', '}', 0, 0,
	'\n', '\n', '\n' ,'\n',//enter
	0, 0, 0, 0, //left control
	'a', 'A', 0, 0,
	's', 'S', 0, 0,
	'd', 'D', 0, 0,
	'f', 'F', 0, 0,
	'g', 'G', 0, 0,
	'h', 'H', 0, 0,
	'j', 'J', 0, 0,
	'k', 'K', 0, 0,
	'l', 'L', 0, 0,
	';', ':', 0, 0,
	'\'', '"', 0, 0,
	'`', '~', 0, 0,
	0, 0, 0, 0, //left shift 0x2A
	'\\', '|', 0, 0,
	'z', 'Z', 0, 0,
	'x', 'X', 0, 0,
	'c', 'C', 0, 0,
	'v', 'V', 0, 0,
	'b', 'B', 0, 0,
	'n', 'N', 0, 0,
	'm', 'M', 0, 0,
	',', '<', 0, 0,
	'.', '>', 0, 0,
	'/', '?', 0, 0,
	0, 0, 0, 0,//right shift
	0, 0, 0, 0,//unknow 0x37
	0, 0, 0, 0,//left alt(option)
	' ', ' ', ' ', ' ',
	0, 0, 0, 0,//caps lock 0x3A
	0, 0, 0, 0,//F1
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,//F5
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,//F10 0x44
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,//0x4B
	// and others
};

char Keyboard::getCharFromKey(int key)
{
	static bool leftShift = false;
	static bool rightShift = false;
	static bool capsLock = false;
	
	if (key == 0x2A)
			leftShift = true;
	else if (key == 0x36)
			rightShift =true;
	else if (key == 0x3A)
			capsLock = !capsLock;
	else if (key == -0x2A)
			leftShift = false;
	else if (key == -0x36)
			rightShift = false;

	if (key < 0)
			return '\0';

	size_t index = key << 2 | ((leftShift || rightShift) ^capsLock);
	if (index < sizeof(KBLAYOUT_US))
			return KBLAYOUT_US[index];

	return '\0';
}
