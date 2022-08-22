#ifndef IO_H_
#define IO_H_
#include <stdint.h>
#include <arch/x86_64.h>
#include <arch/aarch64.h>
uint32_t inb(uint64_t address);
void outb(uint64_t address, uint32_t data);
uint32_t inl(uint64_t address);
void outl(uint64_t address, uint32_t data);
#endif
