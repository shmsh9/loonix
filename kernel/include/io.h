#ifndef IO_H_
#define IO_H_
#include <stdint.h>
uint32_t inb(uint64_t address);
void outb(uint64_t address, uint32_t data);
#endif
