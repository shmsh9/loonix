#ifndef CONSOLE_H_
#define CONSOLE_H_
#include <kstd.h>
#include <graphics/font8x8.h>
#include <drivers/framebuffer.h>
void vt100_console_putchar(framebuffer_device *fb, uint8_t c);
void vt100_console_increase_x(framebuffer_device *fb);
void vt100_console_increase_y(framebuffer_device *fb);
void vt100_console_reset_x(framebuffer_device *fb);
void vt100_console_reset_y(framebuffer_device *fb);
#endif
