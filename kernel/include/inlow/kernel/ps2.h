#ifndef KERNEL_PS2_H__
#define KERNEL_PS2_H__

namespace PS2 {
void initialize();
}

class PS2Device {
public:
    virtual void irqHandler() = 0;
    virtual ~PS2Device() {}
};

#endif