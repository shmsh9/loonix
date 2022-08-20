#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_
#include <graphics/graphics.h>
#include <kstd.h>

typedef graphics_pixel framebuffer_pixel;
typedef struct _framebuffer_device {
    graphics_pixel *buffer;
    graphics_pixel *double_buffer;
    uint16_t width;
    uint16_t height;
    uint64_t size;
    uint16_t flags;
} framebuffer_device;

#define FRAMEBUFFER_DIRECT_WRITE        0x0001
#define FRAMEBUFFER_DOUBLE_BUFFERING    0x0002


framebuffer_device framebuffer_new_device(uintptr_t address, uint64_t width, uint64_t height, uint64_t size, uint16_t flags);
void framebuffer_clear(framebuffer_device *framebuffer, framebuffer_pixel *pixel);
void framebuffer_draw_pixel(framebuffer_device *framebuffer, uint64_t x, uint64_t y, framebuffer_pixel *pixel);
void framebuffer_draw_sprite(framebuffer_device *framebuffer, uint64_t x, uint64_t y, graphics_sprite *sprite);
void framebuffer_draw_sprite_fast(framebuffer_device *framebuffer, uint64_t x, uint64_t y, graphics_sprite *sprite);
void framebuffer_free_device(framebuffer_device *framebuffer);
void framebuffer_update_device(framebuffer_device *framebuffer);
void framebuffer_update_device_partial(framebuffer_device *framebuffer, uint64_t offset, uint64_t size);
void framebuffer_scroll_down(framebuffer_device *framebuffer, uint64_t y);
#endif
