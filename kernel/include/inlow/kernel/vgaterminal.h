#ifndef KERNEL_VGATERMINAL_H
#define KERNEL_VGATERMINAL_H

namespace VgaTerminal
{
	void backspace();
	void printCharacter(char c);
	void printCharacterRaw(char c);
	void updateCursorPosition();
}

#endif
