#include <stddef.h>
#include <inlow/kbkeys.h>
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

static const struct
{
	int key;
	const char* sequence;
} sequences[] = 
{
	{ KB_UP, "\e[A" },
	{ KB_DOWN, "\e[B" },
	{ KB_RIGHT, "\e[C" },
	{ KB_LEFT, "\e[D" },
	{ KB_END, "\e[F" },
	{ KB_HOME, "\e[H" },
	{ KB_INSERT, "\e[2~" },
	{ KB_DELETE, "\e[3~" },
    { KB_PAGEUP, "\e[5~" },
    { KB_PAGEDOWN, "\e[6~" },
    { KB_F1, "\e[OP" },
    { KB_F2, "\e[OQ" },
    { KB_F3, "\e[OR" },
    { KB_F4, "\e[OS" },
    { KB_F5, "\e[15~" },
    { KB_F6, "\e[17~" },
    { KB_F7, "\e[18~" },
    { KB_F8, "\e[19~" },
    { KB_F9, "\e[20~" },
    { KB_F10, "\e[21~" },
    { KB_F11, "\e[23~" },
    { KB_F12, "\e[24~" },
    { 0, 0 }
};

char Keyboard::getCharFromKey(int key)
{
	static bool leftShift = false;
	static bool rightShift = false;
	static bool capsLock = false;
	static bool leftControl = false;
	static bool rightControl = false;
	
	if (key == KB_LSHIFT)
			leftShift = true;
	else if (key == KB_RSHIFT)
			rightShift =true;
	else if (key == KB_CAPSLOCK)
			capsLock = !capsLock;
	else if (key == KB_LCONTROL)
			leftControl = true;
	else if (key == KB_RCONTROL)
			rightControl = true;
	else if (key == -KB_LSHIFT)
			leftShift = false;
	else if (key == -KB_RSHIFT)
			rightShift = false;
	else if (key == -KB_LCONTROL)
			leftControl = false;
	else if (key == -KB_RCONTROL)
			rightControl = false;

	if (key < 0)
			return '\0';

	char result = '\0';
	size_t index = key << 2 | ((leftShift || rightShift) ^capsLock);
	if (index < sizeof(KBLAYOUT_US))
			result =  KBLAYOUT_US[index];
	else if (key == KB_NUMPAD_ENTER)
			result = '\n';
	else if (key == KB_NUMPAD_DIV)
			result = '/';

	if (leftControl || rightControl)
			result = result & 0x1F;

	return result;
}

const char* Keyboard::getSequenceFromKey(int key)
{
	for (size_t i = 0; sequences[i].key != 0; i++)
	{
		if (sequences[i].key == key)
				return sequences[i].sequence;
	}

	return NULL;
}
