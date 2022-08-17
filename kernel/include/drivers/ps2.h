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
ps2_device ps2_device_new(uintptr_t base_port);
uint8_t ps2_device_get_scancode(ps2_device *ps2);
uint8_t ps2_device_send_command(ps2_device *ps2, uint8_t cmd);
uint8_t ps2_scancode_set_1_to_char(uint8_t scancode);
uint8_t ps2_device_getchar(ps2_device *ps2);
#endif
