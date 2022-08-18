#ifndef AARCH64_H_
#define AARCH64_H_
#include <arch/arch.h>
#ifdef __aarch64__
	#define ARCH_STRING "aarch64"
    #define ARCH_UINT 0x01
	#define SERIAL_ADDRESS (uint64_t)0x9000000
    #define SERIAL_RX_TX_OFFSET 0x18
    #define SERIAL_TX_MASK  (1 << 5)
    #define SERIAL_RX_MASK (1 << 4)
    #define PS2_DEVICE_ADDRESS 0x06
    #define JUMP_INSTRUCTION "b"
    #define INTERRUPT_INSTRUCTION "svc #0"
    #define GET_STACKFRAME(stk) __asm__ __volatile__("mov %0, x29" : "=r"(stk))
    #define _INB(address, ret) ret = *(volatile uint32_t *)address
    #define _OUTB(address, data) *(volatile uint32_t *)address = data;
#endif
#endif
