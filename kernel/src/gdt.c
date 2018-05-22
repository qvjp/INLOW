#include <inlow/kernel/gdt.h>

/**
 * x86支持两种虚拟内存方案
 * 分段 必须有的，用GDT管理
 * 分页 可选
 * 
 * 我们的操作系统打算使用分页，但分段不可以也不能关闭
 * 所以这里的解决方法是创建2个大的互相重叠的段（和NULL segment一起）
 * 这些段都是从0x00000000开始到0xFFFFFFFF，分别是特权数据段和特权代码段（kernel）
 * 后期还会增加非特权数据段和非特权代码段（userspace）
 * 这样将打开虚拟内存并相当于禁用了段的效果
 */
struct gdt_entry gdt[] = {
    // NUll Segment
    GDT_ENTRY(0,0,0,0),

    // Kernel Code Segment access: 10011010, flags: 1100
    // 第二个实体是代码段，基地址是0，最大地址单元4GBytes，粒度是4Kbyte，使用32位保护模式
    GDT_ENTRY(0, 0xFFFFFFFF,GDT_PRESENT | GDT_RING0 | GDT_EXECUTABLE | GDT_READ_WRITE,
              GDT_GRANULARITY_4K | GDT_PROTECTED_MODE),

    // Kernel Data Segment access: 10010010, flags: 1100
    // 第三个是数据段，和代码段类似
    GDT_ENTRY(0, 0xFFFFFFFF, GDT_PRESENT | GDT_RING0 | GDT_READ_WRITE,
              GDT_GRANULARITY_4K | GDT_PROTECTED_MODE),
};

/**
 * 汇编代码lgdt将会加载这里
 */
struct gdt_ptr gdt_descriptor = {
    sizeof(gdt) -1,
    gdt
};
