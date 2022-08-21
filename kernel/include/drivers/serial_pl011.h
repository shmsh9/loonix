#ifndef SERIAL_PL011_H_
#define SERIAL_PL011_H_
#include <drivers/serial.h>
#define SERIAL_PL011_ADDRESS (uint64_t)0x9000000
#define SERIAL_PL011_RX_TX_OFFSET 0x18
#define SERIAL_PL011_TX_MASK  (1 << 5)
#define SERIAL_PL011_RX_MASK (1 << 4)
serial_device *serial_pl011_device_new();
void serial_pl011_device_wait_rx(serial_device *serial);
void serial_pl011_device_wait_tx(serial_device *serial);
uint8_t serial_pl011_device_getchar_non_blocking(serial_device *serial);
#endif
