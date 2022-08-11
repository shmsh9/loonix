#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_
#include <kstd.h>

typedef struct {
    uint8_t    Blue;
    uint8_t    Green;
    uint8_t    Red;
    uint8_t    Reserved;
} framebuffer_pixel;

typedef struct {
    framebuffer_pixel *buffer;
    framebuffer_pixel *double_buffer;
    uint16_t width;
    uint16_t height;
    uint64_t size;
} framebuffer_device;

framebuffer_device framebuffer_new_device(uintptr_t address, uint16_t width, uint16_t height, uint64_t size);
void framebuffer_draw_pixel(framebuffer_device *framebuffer, uint16_t x, uint16_t y, framebuffer_pixel *pixel);
void framebuffer_free_device(framebuffer_device *framebuffer);
void framebuffer_update_device(framebuffer_device *framebuffer);
#endif
