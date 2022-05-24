#ifndef SERIALX86_H
#define SERIALX86_H
#include <serial.h>

struct serial_port serialx86_new(uint16_t port);
void serialx86_putchar(struct serial_port serial, char b);
char serialx86_getchar(struct serial_port serial);
int serialx86_init(struct serial_port serial);
void __x86_outb(uint16_t port, uint8_t b);
uint8_t __x86_inb(uint16_t port);

#endif

