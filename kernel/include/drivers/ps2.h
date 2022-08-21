#ifndef PS2_H_
#define PS2_H_
#include <stdint.h>
typedef struct {
    uint64_t data_port;
    uint64_t status_register;
    uint64_t command_register;
} ps2_device;

#include <io.h>
#include <kstd.h>
#define PS2_DEVICE_OUTPUT     0
#define PS2_DEVICE_INPUT      1
#define PS2_DEVICE_SYSTEM     2
#define PS2_DEVICE_COMMAND    3
#define PS2_DEVICE_UNKNOWN    4
#define PS2_DEVICE_UNKNOWN2   5
#define PS2_DEVICE_TIMEOUT    6
#define PS2_DEVICE_PARITY_ERR 7

ps2_device *ps2_device_new(uintptr_t base_port);
uint8_t ps2_device_get_scancode(ps2_device *ps2);
uint8_t ps2_device_send_command(ps2_device *ps2, uint8_t cmd);
uint8_t ps2_scancode_set_1_to_char(uint8_t scancode);
uint8_t ps2_device_getchar(ps2_device *ps2);
uint8_t ps2_device_getchar_non_blocking(ps2_device *ps2);
uint8_t ps2_device_send_command(ps2_device *ps2, uint8_t cmd);
uint8_t ps2_device_send_subcommand(ps2_device *ps2, uint8_t cmd);
void ps2_device_set_bit(ps2_device *ps2, uint8_t bit);
uint8_t ps2_device_get_bit(ps2_device *ps2, uint8_t bit);
void ps2_device_unset_bit(ps2_device *ps2, uint8_t bit);
void ps2_device_wait_tx(ps2_device *ps2);
void ps2_device_wait_rx(ps2_device *ps2);
void ps2_device_end_tx_rx(ps2_device *ps2);
#endif
