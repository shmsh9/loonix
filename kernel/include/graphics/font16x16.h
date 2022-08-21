#ifndef FONT16X16_H_
#define FONT16X16_H_
#include <graphics/graphics.h>
#include <drivers/framebuffer.h>
#include <graphics/font8x8.h>
typedef struct _framebuffer_device framebuffer_device;
void font16x16_draw_framebuffer(char **font8x8, framebuffer_device *fb, uint64_t x, uint64_t y, uint8_t c);
char **font16x16_new();
void font16x16_free(char **font);
#endif

