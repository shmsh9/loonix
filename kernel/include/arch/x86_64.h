#ifndef X86_64_H_
#define X86_64_H_
#ifdef __x86_64__
	#define ARCH "x64"
	#define SERIAL_ADDRESS (uint16_t)0x3f8
    #define PS2_DEVICE_ADDRESS 0x60
    #define JUMP_INSTRUCTION "jmp"
    #define INTERRUPT_INSTRUCTION "int3"
    #define GET_STACKFRAME(stk) __asm__ __volatile__("mov %%rbp, %0" : "=r"(stk))
    #define SERIAL_INIT() outb(SERIAL_ADDRESS, 0x00)

#endif
#endif
