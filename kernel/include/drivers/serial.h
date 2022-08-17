#ifndef SERIAL_DRIVER_H_
#define SERIAL_DRIVER_H_
#include <stdint.h>
#include <io.h>
#include <drivers/pl011uart.h>
#include <arch/aarch64.h>
#include <arch/x86_64.h>
uint8_t SERIAL_READCHAR();
void SERIAL_PUTCHAR(uint8_t c);
void SERIAL_WAITTX();
void SERIAL_WAITRX();
#endif
