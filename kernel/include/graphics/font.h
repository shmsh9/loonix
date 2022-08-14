#ifndef GRAPHICS_FONT_H_
#define GRAPHICS_FONT_H_

#include <kstd.h>
#include <graphics/graphics.h>
#include <drivers/framebuffer.h>

#define ASCII_CHARSET_SIZE 128
#ifndef GRAPHICS_FONT_C_
    extern graphics_sprite font_pixels_ascii[ASCII_CHARSET_SIZE];
#endif
#define FONT_ASCII_WIDTH  4
#define FONT_ASCII_HEIGHT 4
void font_ascii_init();
void font_ascii_draw_framebuffer(framebuffer_device *fb, uint64_t x, uint64_t y, char c);
#endif
