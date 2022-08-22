#ifndef AARCH64_H_
#define AARCH64_H_
#include <arch/arch.h>
#include <stdint.h>
#ifdef __aarch64__
	#define ARCH_STRING "aarch64"
    #define ARCH_UINT ARCH_AARCH64
    #define SERIAL_DEVICE_NEW() serial_pl011_device_new()
    #define PS2_DEVICE_ADDRESS 0x0 //no ps/2 port for Virt
    #define PCI_BUS_CONFIG 0x0
    #define PCI_BUS_ADDRESS 0x0
    #define JUMP_INSTRUCTION "b"
    #define INTERRUPT_INSTRUCTION "svc #0"
    #define GET_STACKFRAME(stk) __asm__ __volatile__("mov %0, x29" : "=r"(stk))
    #define _INB(address, ret) ret = *(volatile uint32_t *)address
    #define _INW(addess, ret) _INB(address, ret)
    #define _INL(addess, ret) _INB(address, ret)
    #define _OUTB(address, data) *(volatile uint32_t *)address = data;
    #define _OUTL(address, data) _OUTB(address, data)
    #define INIT_VECTOR_TABLES()
    #define NEWMEM_HACK_UGLY_OFFSET 0x100000
    #define NEWMEM_ALIGN 0x10
    #define VT100_REFRESH_TICK 0x200000
    typedef struct __attribute__((__packed__)){
        uint64_t x0;
        uint64_t x1;
        uint64_t x2;
        uint64_t x3;
        uint64_t x4;
        uint64_t x5;
        uint64_t x6;
        uint64_t x7;
        uint64_t x8;
        uint64_t x9;
        uint64_t x10;
        uint64_t x11;
        uint64_t x12;
        uint64_t x13;
        uint64_t x14;
        uint64_t x15;
        uint64_t x16;
        uint64_t x17;
        uint64_t x18;
        uint64_t x19;
        uint64_t x20;
        uint64_t x21;
        uint64_t x22;
        uint64_t x23;
        uint64_t x24;
        uint64_t x25;
        uint64_t x26;
        uint64_t x27;
        uint64_t x28;
        uint64_t x29;
        uint64_t x30;
    }cpu_registers;
    #define CPU_REGISTERS_PRINT(regs){\
        for(uint8_t i = 0; i < sizeof(cpu_registers)/sizeof(uint64_t); i++){\
            kprintf("\t[x%d%s] : 0x%x\n", i, i < 10 ? " " : "" ,((uint64_t *)regs)[i]);\
        }\
    }
    void cpu_registers_save(cpu_registers *regs);
    void cpu_registers_load(cpu_registers *regs);
    uint64_t cpu_get_tick();
    void __memset_64b(void *ptr, uint64_t b, uint64_t sz);
    void __memset_128b(void *ptr, __uint128_t b, uint64_t sz);
    void __memcpy_64b(void *dst, void *src, uint64_t sz);
    void __memcpy_128b(void *dst, void *src, uint64_t sz);
    #define __FASTEST_MEMCPY(dst, src, sz) __memcpy_128b(dst, src, sz)
    #define __FASTEST_MEMSET(ptr, b, sz) __memset_64b(ptr, B_to_8B(b), sz)
#endif
#endif
