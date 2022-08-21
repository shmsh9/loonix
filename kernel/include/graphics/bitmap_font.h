#ifndef BITMAP_FONT_H
#define BITMAP_FONT_H
#include <graphics/graphics.h>
#include <drivers/framebuffer.h>

typedef struct _framebuffer_device framebuffer_device;
typedef char ** bitmap_font;
void bitmap_font_free(bitmap_font font);
bitmap_font bitmap_font_new();
void font8x8_draw_framebuffer(bitmap_font font, framebuffer_device *fb, uint64_t x, uint64_t y, uint8_t c);
void bitmap_font_16x16_draw_framebuffer(bitmap_font font16x16, framebuffer_device *fb, uint64_t x, uint64_t y, uint8_t c);
void bitmap_font_8x8_draw_framebuffer(bitmap_font font8x8, framebuffer_device *fb, uint64_t x, uint64_t y, uint8_t c);
#endif

