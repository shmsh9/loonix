#ifndef SERIAL_X86_H
#define SERIAL_X86_H
#include <drivers/serial.h>
#define SERIAL_X86_ADDRESS (uint16_t)0x3f8
#define SERIAL_X86_RX_TX_OFFSET 0x5
#define SERIAL_X86_TX_MASK 0x20
#define SERIAL_X86_RX_MASK 0x1
serial_device *serial_x86_device_new();
void serial_x86_device_wait_rx(serial_device *serial);
void serial_x86_device_wait_tx(serial_device *serial);
uint8_t serial_x86_device_getchar_non_blocking(serial_device *serial);
#endif
