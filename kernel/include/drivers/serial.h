#ifndef SERIAL_DRIVER_H_
#define SERIAL_DRIVER_H_
#include <stdint.h>

#ifdef __x86_64__
	#define SERIAL_ADDRESS (uint16_t)0x3f8
	#define SERIAL_PUTCHAR(b) __asm__ __volatile__ ("outb %0, %1" : : "a"((uint8_t)b), "Nd"(SERIAL_ADDRESS) )
#endif
#ifdef __aarch64__
	#define SERIAL_ADDRESS 0x9000000
	#define SERIAL_PUTCHAR(b) (*(volatile uint32_t *)SERIAL_ADDRESS = (volatile uint32_t)b);
#endif
#define SERIAL_PUTS(s) while(*s){SERIAL_PUTCHAR(*s);}

#endif
