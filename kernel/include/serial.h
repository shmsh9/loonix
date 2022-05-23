#ifndef SERIAL_H_
#define SERIAL_H_
#include <stdint.h>
#include <drivers/pl011.h>

#ifdef __x86_64__
	#define SERIAL_PORT 0x3f8
#endif
#ifdef __aarch64__
  #define SERIAL_PORT glob_pl011
#endif
void outb(uint8_t val);
uint8_t inb();
int init_serial();
void write_serial(char a);
void puts_serial(const char *str);
void wait_transmission();
void __x86_outb(uint16_t port, uint8_t b);
uint8_t __x86_inb(uint16_t port);
#endif
