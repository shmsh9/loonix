#ifndef X86_64_H_
#define X86_64_H_
#include <arch/arch.h>
#ifdef __x86_64__
	#define ARCH_STRING "x64"
    #define ARCH_UINT 0x0
	#define SERIAL_ADDRESS (uint16_t)0x3f8
    #define SERIAL_RX_TX_OFFSET 0x5
    #define SERIAL_TX_MASK 0x20
    #define SERIAL_RX_MASK 0x1
    #define PS2_DEVICE_ADDRESS 0x60
    #define JUMP_INSTRUCTION "jmp"
    #define INTERRUPT_INSTRUCTION "int3"
    #define GET_STACKFRAME(stk) __asm__ __volatile__("mov %%rbp, %0" : "=r"(stk))
#endif
#endif
