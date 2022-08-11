#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_
#include <kstd.h>
#include <graphics.h>

#define FRAMEBUFFER_DIRECT_WRITE        0x0001
#define FRAMEBUFFER_DOUBLE_BUFFERING    0x0002

typedef graphics_pixel framebuffer_pixel;

typedef struct {
    framebuffer_pixel *buffer;
    framebuffer_pixel *double_buffer;
    uint16_t width;
    uint16_t height;
    uint64_t size;
    uint16_t flags;
} framebuffer_device;

framebuffer_device framebuffer_new_device(uintptr_t address, uint16_t width, uint16_t height, uint64_t size, uint16_t flags);
void framebuffer_clear(framebuffer_device *framebuffer, framebuffer_pixel *pixel);
void framebuffer_draw_pixel(framebuffer_device *framebuffer, uint16_t x, uint16_t y, framebuffer_pixel *pixel);
void framebuffer_draw_sprite(framebuffer_device *framebuffer, uint16_t x, uint16_t y, graphics_sprite *sprite);
void framebuffer_free_device(framebuffer_device *framebuffer);
void framebuffer_update_device(framebuffer_device *framebuffer);
#endif
