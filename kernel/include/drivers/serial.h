#ifndef SERIAL_DRIVER_H_
#define SERIAL_DRIVER_H_
#include <stdint.h>
typedef struct{
	uint64_t data;
	uint64_t rx_tx;
	uint8_t  rx_mask;
	uint8_t  tx_mask;
	uint8_t  arch;
}serial_device;
#include <io.h>
#include <drivers/pl011uart.h>
#include <arch/aarch64.h>
#include <arch/x86_64.h>
#include <kstd.h>
serial_device serial_device_new();
void serial_device_init(serial_device *serial);
uint8_t serial_device_readchar(serial_device *serial);
void serial_device_waittx(serial_device *serial);
void serial_device_waitrx(serial_device *serial);
void serial_device_putchar(serial_device *serial, uint8_t c);
uint8_t serial_device_readchar_non_blocking(serial_device *serial);
#endif
