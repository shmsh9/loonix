#ifndef PL011_H_
#define PL011_H_
#include <stdint.h>
struct pl011 {
    uint64_t base_address;
    uint64_t base_clock;
    uint32_t baudrate;
    uint32_t data_bits;
    uint32_t stop_bits;
};
extern struct pl011 glob_pl011;
#define FR_BUSY      (1 << 3)
#define CR_TXEN      (1 << 8)
#define CR_UARTEN    (1 << 0)
#define LCR_FEN      (1 << 4)
#define LCR_STP2     (1 << 3)
#define DR_OFFSET    0x000
#define FR_OFFSET    0x018
#define IBRD_OFFSET  0x024
#define FBRD_OFFSET  0x028
#define LCR_OFFSET   0x02c
#define CR_OFFSET    0x030
#define IMSC_OFFSET  0x038
#define DMACR_OFFSET 0x048

volatile uint32_t *reg(const struct pl011 *dev, uint32_t offset);
void calculate_divisors( const struct pl011 *dev, uint32_t *integer, uint32_t *fractional);
void wait_tx_complete(const struct pl011 *dev);
int pl011_setup(struct pl011 *dev, uint64_t base_address, uint64_t base_clock);
int pl011_reset(const struct pl011 *dev);
int pl011_send(const struct pl011 *dev, const char b);

#endif

