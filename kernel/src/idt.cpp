#include <stdint.h>

extern "C"
{
	//CPU Exceptions
	extern void isr_0(void);
	extern void isr_1(void);
	extern void isr_2(void);
	extern void isr_3(void);
	extern void isr_4(void);
	extern void isr_5(void);
	extern void isr_6(void);
	extern void isr_7(void);
	extern void isr_8(void);
	extern void isr_9(void);
	extern void isr_10(void);
	extern void isr_11(void);
	extern void isr_12(void);
	extern void isr_13(void);
	extern void isr_14(void);
	extern void isr_15(void);
	extern void isr_16(void);
	extern void isr_17(void);
	extern void isr_18(void);
	extern void isr_19(void);
	extern void isr_20(void);

	//IRQs
	extern void isr_32(void);
	extern void isr_33(void);
	extern void isr_34(void);
	extern void isr_35(void);
	extern void isr_36(void);
	extern void isr_37(void);
	extern void isr_38(void);
	extern void isr_39(void);
	extern void isr_40(void);
	extern void isr_41(void);
	extern void isr_42(void);
	extern void isr_43(void);
	extern void isr_44(void);
	extern void isr_45(void);
	extern void isr_46(void);
	extern void isr_47(void);
}

struct idt_entry
{
	uint16_t offset_low;
	uint16_t selector;
	uint8_t unused;
	uint8_t flags;
	uint16_t offset_high;
};

#define IDT_ENTRY(offset, selector, flags) \
{ \
	(uint16_t)((uintptr_t)(offset) & 0xFFFF), \
	(selector), \
	0, \
	(flags), \
	(uint16_t)(((uintptr_t)(offset) >> 16) & 0xFFFF) \
}

#define IDT_INTERRUPT_GATE 0xE
#define IDT_RING0 0
#define IDT_PRESENT (1 << 7)

idt_entry idt[] = {
	IDT_ENTRY(isr_0, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_1, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_2, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_3, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_4, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_5, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_6, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_7, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_8, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_9, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_10, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_11, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_12, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_13, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_14, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(0,0,0),
	IDT_ENTRY(isr_16, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_17, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_18, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_19, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_20, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),

	IDT_ENTRY(0,0,0),/*[21]*/
	IDT_ENTRY(0,0,0),
	IDT_ENTRY(0,0,0),
	IDT_ENTRY(0,0,0),
	IDT_ENTRY(0,0,0),
	IDT_ENTRY(0,0,0),
	IDT_ENTRY(0,0,0),
	IDT_ENTRY(0,0,0),
	IDT_ENTRY(0,0,0),
	IDT_ENTRY(0,0,0),
	IDT_ENTRY(0,0,0),/*[31]*/

	IDT_ENTRY(isr_32, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_33, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_34, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_35, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_36, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_37, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_38, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_39, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_40, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_41, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_42, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_43, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_44, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_45, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_46, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
	IDT_ENTRY(isr_47, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT),
};

extern "C"
{
	struct
	{
		uint16_t size;
		void* idtp;
	} __attribute__((__packed__)) idt_descriptor = 
	{
		sizeof(idt) -1,
		idt
	};
}
