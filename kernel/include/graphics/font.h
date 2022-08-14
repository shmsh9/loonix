#ifndef GRAPHICS_FONT_H_
#define GRAPHICS_FONT_H_
#include <graphics/graphics.h>
#include <kstd.h>
#define ASCII_CHARSET_SIZE 128
#ifndef GRAPHICS_FONT_C_
    extern graphics_sprite font_pixels_ascii[ASCII_CHARSET_SIZE];
#endif
#define FONT_ASCII_WIDTH  4
#define FONT_ASCII_HEIGHT 4
void font_ascii_init();
#endif
