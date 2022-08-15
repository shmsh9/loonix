#ifndef FONT8X8_H_
#define FONT8X8_H_

#include <drivers/framebuffer.h>
void font8x8_draw_framebuffer(framebuffer_device *fb, uint64_t x, uint64_t y, uint8_t c);

#endif

