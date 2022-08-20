#ifndef GRAPHICS_H_
#define GRAPHICS_H_
#include <stdint.h>
#define GRAPHICS_PIXEL_RED  (graphics_pixel){.Blue = 0x00, .Green = 0x00, .Red = 0xff, .Alpha = 0xff}
#define GRAPHICS_PIXEL_GREEN (graphics_pixel){.Blue = 0x00, .Green = 0xff, .Red = 0x00, .Alpha = 0xff}
#define GRAPHICS_PIXEL_BLUE  (graphics_pixel){.Blue = 0xff, .Green = 0x00, .Red = 0x00, .Alpha = 0xff}
#define GRAPHICS_PIXEL_WHITE (graphics_pixel){.Blue = 0xff, .Green = 0xff, .Red = 0xff, .Alpha = 0xff}
#define GRAPHICS_PIXEL_GRAY (graphics_pixel){.Blue = 0x80, .Green = 0x80, .Red = 0x80, .Alpha = 0xff}
#define GRAPHICS_PIXEL_BLACK (graphics_pixel){.Blue = 0x00, .Green = 0x00, .Red = 0x00, .Alpha = 0xff}
#define GRAPHICS_PIXEL_TRANSPARENT (graphics_pixel){.Blue = 0x00, .Green = 0x00, .Red = 0x00, .Alpha = 0x00}

#define GRAPHICS_CENTER_WIDTH(fb, sprite) ((fb.width/2) - (sprite.width/2))
#define GRAPHICS_CENTER_HEIGHT(fb, sprite) ((fb.height/2) - (sprite.height/2))
typedef struct {
    uint8_t    Blue;
    uint8_t    Green;
    uint8_t    Red;
    uint8_t    Alpha;
} graphics_pixel;

typedef struct{
    graphics_pixel *pixels;
    uint64_t width;
    uint64_t height;
} graphics_sprite;
#include <kstd.h>
graphics_sprite *graphics_sprite_new(uint64_t x, uint64_t y);
graphics_sprite *graphics_sprite_static_new(uint64_t x, uint64_t y, uint8_t *pixels);
void graphics_sprite_free(graphics_sprite *ptr);
void graphics_sprite_static_free(graphics_sprite *ptr);
#endif
