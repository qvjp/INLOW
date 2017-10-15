#ifndef KERNEL_PS2_H
#define KERNEL_PS2_H

namespace PS2
{
	void initialize();
}

class PS2Device
{
	public:
			virtual void irqHandler() = 0;
			virtual ~PS2Device(){}
};

#endif
