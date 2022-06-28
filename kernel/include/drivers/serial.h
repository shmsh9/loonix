#ifndef SERIAL_DRIVER_H_
#define SERIAL_DRIVER_H_
#include <stdint.h>
#include <io.h>

uint8_t SERIAL_READCHAR();
void SERIAL_PUTCHAR(char c);
void SERIAL_WAITCHAR();
void SERIAL_INIT();
#ifdef __x86_64__
	#define SERIAL_ADDRESS (uint16_t)0x3f8
#endif
#ifdef __aarch64__
	#define SERIAL_ADDRESS (uint64_t)0x9000000
#endif

#endif
