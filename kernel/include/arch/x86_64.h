#ifndef X86_64_H_
#define X86_64_H_
#include <arch/arch.h>
#include <stdint.h>
#ifdef __x86_64__
	#define ARCH_STRING "x64"
    #define ARCH_UINT ARCH_X64
	#define SERIAL_ADDRESS (uint16_t)0x3f8
    #define SERIAL_RX_TX_OFFSET 0x5
    #define SERIAL_TX_MASK 0x20
    #define SERIAL_RX_MASK 0x1
    #define PS2_DEVICE_ADDRESS 0x60
    #define JUMP_INSTRUCTION "jmp"
    #define INTERRUPT_INSTRUCTION "int3"
    #define GET_STACKFRAME(stk) __asm__ __volatile__("mov %%rbp, %0" : "=r"(stk))
    #define _INB(address, ret){\
        volatile uint8_t tmp = 0;\
        __asm__ __volatile__("inb %1, %0" :  "=a"(tmp) : "dN"((volatile uint16_t)address));\
        ret = tmp;\
    }
    #define _OUTB(address, data) __asm__ __volatile__("outb %0, %1" : : "a"((uint8_t)data), "Nd"((uint16_t)address))
    #define NEWMEM_HACK_UGLY_OFFSET 0x0
    #define NEWMEM_ALIGN 0x10
    #define VT100_REFRESH_TICK 0xfffff

    typedef struct{
        uint64_t rax;
        uint64_t rbx;
        uint64_t rcx;
        uint64_t rdx;
        uint64_t rsi;
        uint64_t rdi;
        uint64_t rbp;
        uint64_t rsp;
        uint64_t r8;
        uint64_t r9;
        uint64_t r10;
        uint64_t r11;
        uint64_t r12;
        uint64_t r13;
        uint64_t r14;
        uint64_t r15;
    }cpu_registers;
    #define CPU_REGISTERS_PRINT(regs){\
        char *cpu_registers_names__func__ [sizeof(cpu_registers)/sizeof(uint64_t)] = {0};\
        cpu_registers_names__func__[0] = "rax";\
        cpu_registers_names__func__[1] = "rbx";\
        cpu_registers_names__func__[2] = "rcx";\
        cpu_registers_names__func__[3] = "rdx";\
        cpu_registers_names__func__[4] = "rsi";\
        cpu_registers_names__func__[5] = "rdi";\
        cpu_registers_names__func__[6] = "rbp";\
        cpu_registers_names__func__[7] = "rsp";\
        cpu_registers_names__func__[8] = "r8 ";\
        cpu_registers_names__func__[9] = "r9 ";\
        cpu_registers_names__func__[10] = "r10";\
        cpu_registers_names__func__[11] = "r11";\
        cpu_registers_names__func__[12] = "r12";\
        cpu_registers_names__func__[13] = "r13";\
        cpu_registers_names__func__[14] = "r14";\
        cpu_registers_names__func__[15] = "r15";\
        for(uint8_t i = 0; i < sizeof(cpu_registers)/sizeof(uint64_t); i++){\
            kprintf("\t[%s] : 0x%x\n", cpu_registers_names__func__[i],((uint64_t *)regs)[i]);\
        }\
    }
    void cpu_registers_save(cpu_registers *regs);
    void cpu_registers_load(cpu_registers *regs);
    uint64_t cpu_get_tick();
#endif
#endif
