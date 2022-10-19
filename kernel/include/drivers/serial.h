#ifndef SERIAL_DRIVER_H_
#define SERIAL_DRIVER_H_
#include <stdint.h>
typedef struct _serial_device{
	uint64_t data;
	void (*wait_tx)(struct _serial_device *serial);
	void (*wait_rx)(struct _serial_device *serial);
	uint8_t (*getchar_non_blocking)(struct _serial_device *serial);
	uint64_t rx_tx;
	uint8_t  rx_mask;
	uint8_t  tx_mask;
	uint8_t  char_in;
}serial_device;
#include <io.h>
#include <drivers/pl011uart.h>
#include <arch/aarch64.h>
#include <arch/x86_64.h>
#include <kstd/kstd.h>
#include <drivers/serial_pl011.h>
#include <drivers/serial_x86.h>
int serial_device_init(serial_device *serial);
uint8_t serial_device_readchar(serial_device *serial);
void serial_device_waittx(serial_device *serial);
void serial_device_waitrx(serial_device *serial);
void serial_device_putchar(serial_device *serial, uint8_t c);
uint8_t serial_device_readchar_non_blocking(serial_device *serial);
#endif
