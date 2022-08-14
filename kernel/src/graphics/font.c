#include <graphics/font.h>

#define GRAPHICS_FONT_C_
graphics_sprite font_pixels_ascii[ASCII_CHARSET_SIZE] = {0};
void font_ascii_draw_framebuffer(framebuffer_device *fb, uint64_t x, uint64_t y, char c){
    graphics_sprite sp = font_pixels_ascii[(uint8_t)c];
    framebuffer_draw_sprite(fb, x, y, &sp); 
}
void font_ascii_init(){
    graphics_pixel font_pixels_H[] = {
		    GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_WHITE,
		    GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE,
		    GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE,
		    GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_WHITE,
    };
    graphics_pixel font_pixels_I[] = {
        GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE,
        GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_TRANSPARENT,
        GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_TRANSPARENT,
        GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE,
    };
    graphics_pixel font_pixels_WHITESPACE[] = {
        GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT,
        GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT,
        GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT,
        GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT,
    }
    ;
    graphics_pixel font_pixels_NULL[] = {
        GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_TRANSPARENT,
        GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_TRANSPARENT,
        GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT,
        GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_TRANSPARENT,
    }
    ;
    uint64_t font_pixels_char_size = sizeof(graphics_pixel)*(FONT_ASCII_WIDTH*FONT_ASCII_HEIGHT);
    //memory leak
    graphics_pixel *font_ascii_mem = kcalloc(font_pixels_char_size,ASCII_CHARSET_SIZE);
    if(!font_ascii_mem){
        KERROR("error allocating memory for font");
        return;
    }
    KDEBUG("allocated font_ascii_mem %dKB at 0x%x", 
            BYTES_TO_KB(font_pixels_char_size*ASCII_CHARSET_SIZE),
            font_ascii_mem
            );
    KDEBUG("font_pixels_ascii at 0x%x", font_pixels_ascii);
    for(uint8_t i = 0; i < ASCII_CHARSET_SIZE; i++){
        font_pixels_ascii[i] = (graphics_sprite){
            .width = FONT_ASCII_WIDTH,
            .height = FONT_ASCII_HEIGHT,
            .pixels = (graphics_pixel *)((uintptr_t)font_ascii_mem+(i*font_pixels_char_size))
        };
        memcpy(font_pixels_ascii[i].pixels, font_pixels_NULL, font_pixels_char_size);
    }
    memcpy(font_pixels_ascii['I'].pixels, font_pixels_I, font_pixels_char_size);
    memcpy(font_pixels_ascii['H'].pixels, font_pixels_H, font_pixels_char_size);
    memcpy(font_pixels_ascii[' '].pixels, font_pixels_WHITESPACE, font_pixels_char_size);
}
