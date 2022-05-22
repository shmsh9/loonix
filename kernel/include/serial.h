#ifndef SERIAL_H_
#define SERIAL_H_
#include <stdint.h>

#define PORT 0x3f8          // COM1
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
int init_serial();
int is_transmit_empty();
void write_serial(char a);
void puts_serial(char *str);
#endif
