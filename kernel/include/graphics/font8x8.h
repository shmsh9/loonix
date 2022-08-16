#ifndef FONT8X8_H_
#define FONT8X8_H_
#include <graphics/graphics.h>
#include <drivers/framebuffer.h>
typedef struct _framebuffer_device framebuffer_device;
void font8x8_draw_framebuffer(framebuffer_device *fb, uint64_t x, uint64_t y, uint8_t c);
char **font8x8_new();
void font8x8_free(char **font);

#endif

