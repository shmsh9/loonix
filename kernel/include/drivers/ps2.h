#ifndef PS2_H_
#define PS2_H_
#include <stdint.h>
#include <stdbool.h>
typedef struct {
    uint64_t data_port;
    uint64_t status_register;
    uint64_t command_register;
} ps2_device;

#include <io.h>
#include <kstd/kstd.h>
#define PS2_SET_1_SIZE        0x58
#define PS2_DEVICE_OUTPUT     0
#define PS2_DEVICE_INPUT      1
#define PS2_DEVICE_SYSTEM     2
#define PS2_DEVICE_COMMAND    3
#define PS2_DEVICE_UNKNOWN    4
#define PS2_DEVICE_UNKNOWN2   5
#define PS2_DEVICE_TIMEOUT    6
#define PS2_DEVICE_PARITY_ERR 7

typedef enum _ps2_key_pressed{
    PS2_KEY_NULL,
    PS2_KEY_ESCAPE,
    PS2_KEY_1,
    PS2_KEY_2,
    PS2_KEY_3,
    PS2_KEY_4,
    PS2_KEY_5,
    PS2_KEY_6,
    PS2_KEY_7,
    PS2_KEY_8,
    PS2_KEY_9,
    PS2_KEY_0,
    PS2_KEY_MINUS,
    PS2_KEY_EQUALS,
    PS2_KEY_BACKSPACE,
    PS2_KEY_TAB,
    PS2_KEY_Q,
    PS2_KEY_W,
    PS2_KEY_E,
    PS2_KEY_R,
    PS2_KEY_T,
    PS2_KEY_Y,
    PS2_KEY_U,
    PS2_KEY_I,
    PS2_KEY_O,
    PS2_KEY_P,
    PS2_KEY_SQUARE_BRACKET_OPEN,
    PS2_KEY_SQUARE_BRACKET_CLOSE,
    PS2_KEY_RETURN,
    PS2_KEY_CTRL_LEFT,
    PS2_KEY_A,
    PS2_KEY_S,
    PS2_KEY_D,
    PS2_KEY_F,
    PS2_KEY_G,
    PS2_KEY_H,
    PS2_KEY_J,
    PS2_KEY_K,
    PS2_KEY_L,
    PS2_KEY_SEMI_COLON,
    PS2_KEY_SINGLE_QUOTE,
    PS2_KEY_BACK_TICK,
    PS2_KEY_SHIFT_LEFT,
    PS2_KEY_BACKSLASH,
    PS2_KEY_Z,
    PS2_KEY_X,
    PS2_KEY_C,
    PS2_KEY_V,
    PS2_KEY_B,
    PS2_KEY_N,
    PS2_KEY_M,
    PS2_KEY_COMMA,
    PS2_KEY_FULL_STOP,
    PS2_KEY_SLASH,
    PS2_KEY_SHIFT_RIGHT,
    PS2_KEY_ASTERISK_KEYPAD,
    PS2_KEY_ALT_LEFT,
    PS2_KEY_SPACE,
    PS2_KEY_CAPS_LOCK,
    PS2_KEY_F1,
    PS2_KEY_F2,
    PS2_KEY_F3,
    PS2_KEY_F4,
    PS2_KEY_F5,
    PS2_KEY_F6,
    PS2_KEY_F7,
    PS2_KEY_F8,
    PS2_KEY_F9,
    PS2_KEY_F10,
    PS2_KEY_NUM_LOCK,
    PS2_KEY_SCROLL_LOCK,
    PS2_KEY_7_KEYPAD,
    PS2_KEY_8_KEYPAD,
    PS2_KEY_9_KEYPAD,
    PS2_KEY_MINUS_KEYPAD,
    PS2_KEY_4_KEYPAD,
    PS2_KEY_5_KEYPAD,
    PS2_KEY_6_KEYPAD,
    PS2_KEY_PLUS_KEYPAD,
    PS2_KEY_1_KEYPAD,
    PS2_KEY_2_KEYPAD,
    PS2_KEY_3_KEYPAD,
    PS2_KEY_0_KEYPAD,
    PS2_KEY_FULL_STOP_KEYPAD,
    PS2_KEY_F11 = 0x57,
    PS2_KEY_F12 = 0x58
}ps2_key_pressed;

void ps2_keypress_update(uint8_t scancode);
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
bool ps2_key_is_pressed(ps2_key_pressed k);
void ps2_device_irq_handler();
#endif
