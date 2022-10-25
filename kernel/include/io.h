#ifndef IO_H_
#define IO_H_
#include <stdint.h>
#include <arch/arch.h>
void outb(uint64_t address, uint32_t data);
void outw(uint64_t address, uint32_t data);
void outl(uint64_t address, uint32_t data);
uint32_t inb(uint64_t address);
uint32_t inl(uint64_t address);
uint16_t inw(uint64_t address);
#endif
