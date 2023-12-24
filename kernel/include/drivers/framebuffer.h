#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_
#include <graphics/graphics.h>
#include <kstd/kstd.h>

typedef graphics_pixel framebuffer_pixel;
typedef struct _framebuffer_device {
    graphics_pixel *buffer;
    graphics_pixel *double_buffer;
    graphics_pixel *dst;
    uint16_t width;
    uint16_t height;
    uint64_t size;
    uint16_t flags;
} framebuffer_device;

#define FRAMEBUFFER_DIRECT_WRITE        0x0001
#define FRAMEBUFFER_DOUBLE_BUFFERING    0x0002


framebuffer_device *framebuffer_device_new(uintptr_t address, uint64_t width, uint64_t height, uint16_t flags);
void framebuffer_device_clear(framebuffer_device *framebuffer, framebuffer_pixel *pixel);
void framebuffer_device_draw_pixel_slow(framebuffer_device *framebuffer, int64_t x, int64_t y, framebuffer_pixel *pixel);
void framebuffer_device_draw_pixel_fast(framebuffer_device *framebuffer, uint64_t x, uint64_t y, framebuffer_pixel *pixel);
void framebuffer_device_draw_sprite_slow(framebuffer_device *framebuffer, int64_t x, int64_t y, graphics_sprite *sprite);
void framebuffer_device_draw_sprite_fast(framebuffer_device *framebuffer, int64_t x, int64_t y, graphics_sprite *sprite);
void framebuffer_device_free(framebuffer_device *framebuffer);
void framebuffer_device_update(framebuffer_device *framebuffer);
void framebuffer_device_update_partial(framebuffer_device *framebuffer, uint64_t offset, uint64_t size);
void framebuffer_device_scroll_down(framebuffer_device *framebuffer, uint64_t y);
#endif
