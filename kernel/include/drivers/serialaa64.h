#ifndef SERIALAA64_H_
#define SERIALAA64_H_
#include <serial.h>

#define DR_OFFSET    0x000
#define FR_OFFSET    0x018
#define IBRD_OFFSET  0x024
#define FBRD_OFFSET  0x028
#define LCR_OFFSET   0x02c
#define CR_OFFSET    0x030
#define IMSC_OFFSET  0x038
#define DMACR_OFFSET 0x048
#define CR_TXEN      (1 << 8)
#define CR_UARTEN    (1 << 0)
#define LCR_FEN      (1 << 4)
#define LCR_STP2     (1 << 3)
#define FR_BUSY      (1 << 3)
struct serial_port serialaa64_new(uint64_t port);
int serialaa64_init(struct serial_port serial);
char serialaa64_getchar(struct serial_port serial);
void serialaa64_putchar(struct serial_port serial, char b);
void __aarch64_outb(struct serial_port serial, uint32_t b);
volatile int  __aarch64_inb(struct serial_port serial);
void __aarch64_setreg(uint64_t reg, uint32_t val);
volatile uint32_t __aarch64_getreg(uint64_t reg);
void serialaa64_waittx(struct serial_port serial);
void serialaa64_calculatedivisors(uint32_t *integer, uint32_t *fractional);
#endif

