#ifndef KERNEL_PS2_H
#define KERNEL_PS2_H

#include <stdint.h>

namespace PS2
{
	void initialize();
	uint8_t sendDeviceCommand(uint8_t command);
	uint8_t sendDeviceCommand(uint8_t command, uint8_t data);
}

class PS2Device
{
	public:
			virtual void irqHandler() = 0;
			virtual ~PS2Device(){}
};

#endif
