#ifndef CONSOLE_H_
#define CONSOLE_H_
#include <kstd/kstd.h>
#include <graphics/bitmap_font.h>
#include <drivers/framebuffer.h>
#define VT100_CURSOR_N_FRAME 30
#define VT100_CURSOR_FIRST_FRAME 100
#define VT100_CURSOR_OFF 0x0
#define VT100_CURSOR_LAST_FRAME VT100_CURSOR_FIRST_FRAME+VT100_CURSOR_N_FRAME 

extern bool vt100_console_initialized;
void vt100_set_cursor_char(uint8_t c);
void vt100_console_init(framebuffer_device *fb);
void vt100_console_update_draw_screen(framebuffer_device *fb);
void vt100_console_putchar(framebuffer_device *fb, uint8_t c);
void vt100_console_increase_x(framebuffer_device *fb);
void vt100_console_increase_y(framebuffer_device *fb);
void vt100_console_decrease_x(framebuffer_device *fb);
void vt100_console_set_x(framebuffer_device *fb, uint64_t x);
void vt100_console_set_y(framebuffer_device *fb, uint64_t y);
void vt100_console_escaping_stop(framebuffer_device *fb, uint8_t c);
void vt100_console_reset_x(framebuffer_device *fb);
void vt100_console_reset_y(framebuffer_device *fb);
#endif
