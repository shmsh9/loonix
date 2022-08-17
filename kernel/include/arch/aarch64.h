#ifndef AARCH64_H_
#define AARCH64_H_
#ifdef __aarch64__
	#define ARCH "aarch64"
	#define SERIAL_ADDRESS (uint64_t)0x9000000
    #define PS2_DEVICE_ADDRESS 0x60
    #define JUMP_INSTRUCTION "b"
    #define INTERRUPT_INSTRUCTION "svc #0"
    #define GET_STACKFRAME(stk) __asm__ __volatile__("mov %0, x29" : "=r"(stk))
    #define SERIAL_INIT()
#endif
#endif
