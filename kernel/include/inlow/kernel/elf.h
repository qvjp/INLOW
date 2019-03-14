/** MIT License
 * 
 * Copyright (c) 2019 INLOW-OS
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * kernel/include/inlow/kernel/elf.h
 * 定义ElfHeader和ProgramHeader
 */

#ifndef KERNEL_ELF_H__
#define KERNEL_ELF_H__

#include <stdint.h> /* uint16_t uint32_t */

/**
 * ELF头
 */
struct ElfHeader {
    unsigned char e_ident[16];  /* 包括标示ELF格式的魔幻数字和其他与机器相关的条目 */
    uint16_t e_type;            /* 文件类型 */
    uint16_t e_machine;         /* 运行此程序需要的架构 0x03是x86架构*/
    uint32_t e_version;         /* 程序版本 */
    uint32_t e_entry;           /* 程序入口地址 */
    uint32_t e_phoff;           /* Program header table在文件中的偏移量*/
    uint32_t e_shoff;           /* Section header table在文件中的偏移量*/
    uint32_t e_flags;           /* 与处理器相关的标示 */
    uint16_t e_ehsize;          /* 此ELF头的大小 */
    uint16_t e_phentsize;       /* Program header table项的大小 */
    uint16_t e_phnum;           /* Program header table项的个数 */
    uint16_t e_shentsize;       /* section header table项的大小 */
    uint16_t e_shnum;           /* section header table项的个数 */
    uint16_t e_shstrndx;        /* 包含section header table项名称的字符串是第一个字节 */
};

/**
 * 程序头表
 */
struct ProgramHeader {
    uint32_t p_type;            /* 当前Program header所描述的段的类型。 
                                 * 0 - 忽略
                                 * 1 - 加载 将p_vaddr开始p_memsz长的内存设为0，然后从p_offset
                                 *          开始复制p_filesz个字节到p_vaddr。
                                 * 2 - 动态 需要动态链接
                                 */
    uint32_t p_offset;          /* 段在文件中的偏移 */
    uint32_t p_vaddr;           /* 段的一个字节在内存中的虚拟地址 */
    uint32_t p_paddr;           /* 在物理内存定位相关的系统中，此项是为物理地址保留。 */
    uint32_t p_filesz;          /* 段在文件中的长度 */
    uint32_t p_memsz;           /* 段在内存中的长度 */
    uint32_t p_flags;           /* 段相关标示 */
    uint32_t p_align;           /* 段在文件和内存中如何对齐 */
};

/* ProgramHeader中p_type为1表示加载 */
#define PT_LOAD 1

#endif /* KERNEL_ELF_H__ */
