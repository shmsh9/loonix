#ifndef GRAPHICS_H_
#define GRAPHICS_H_
#include <stdint.h>

#define GRAPHICS_RED_PIXEL   (graphics_pixel){.Blue = 0x00, .Green = 0x00, .Red = 0xff, .Alpha = 0x00}
#define GRAPHICS_GREEN_PIXEL (graphics_pixel){.Blue = 0x00, .Green = 0xff, .Red = 0x00, .Alpha = 0x00}
#define GRAPHICS_BLUE_PIXEL  (graphics_pixel){.Blue = 0xff, .Green = 0x00, .Red = 0x00, .Alpha = 0x00}
#define GRAPHICS_WHITE_PIXEL (graphics_pixel){.Blue = 0xff, .Green = 0xff, .Red = 0xff, .Alpha = 0x00}
#define GRAPHICS_BLACK_PIXEL (graphics_pixel){.Blue = 0x00, .Green = 0x00, .Red = 0x00, .Alpha = 0x00}

typedef struct {
    uint8_t    Blue;
    uint8_t    Green;
    uint8_t    Red;
    uint8_t    Alpha;
} graphics_pixel;

typedef struct{
    graphics_pixel *pixels;
    uint16_t width;
    uint16_t height;
} graphics_sprite;


#endif
