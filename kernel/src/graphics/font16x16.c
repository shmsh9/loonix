#include <graphics/font16x16.h>

char **font16x16_new(){
    return font8x8_new();
}
void font16x16_free(char **font){
    font8x8_free(font);
}
void font16x16_draw_framebuffer(char **font16x16, framebuffer_device *fb, uint64_t x, uint64_t y, uint8_t c){
    if(!font16x16)
        return;
    if(c >= 128){
        KERROR("c is out of ascii range");
        return;
    }
    if(c == ' '){
        uint64_t curr_y = y;
        uint64_t curr_x = x;
        for(uint8_t current_bitfield = 0; current_bitfield < 8; current_bitfield++){
            curr_x = x;
            for(uint8_t current_bit = 0; current_bit < 8; current_bit++){
                framebuffer_device_draw_pixel(fb,
                           curr_x,
                           curr_y,
                           &(graphics_pixel){.Red = 0x00, .Green = 0x00, .Blue = 0x00, .Alpha = 0xff}
                );
                framebuffer_device_draw_pixel(fb,
                           curr_x+1,
                           curr_y,
                           &(graphics_pixel){.Red = 0x00, .Green = 0x00, .Blue = 0x00, .Alpha = 0xff}
                );
                framebuffer_device_draw_pixel(fb,
                           curr_x,
                           curr_y+1,
                           &(graphics_pixel){.Red = 0x00, .Green = 0x00, .Blue = 0x00, .Alpha = 0xff}
                );
                framebuffer_device_draw_pixel(fb,
                           curr_x+1,
                           curr_y+1,
                           &(graphics_pixel){.Red = 0x00, .Green = 0x00, .Blue = 0x00, .Alpha = 0xff}
                );

               curr_x+= 2;
            }
            curr_y += 2;
        }
            return;
    }
    uint64_t curr_y = y;
    uint64_t curr_x = x;
    for(uint8_t current_bitfield = 0; current_bitfield < 8; current_bitfield++){
        curr_x = x;
        for(uint8_t current_bit = 0; current_bit < 8; current_bit++){
           if( get_bit(font16x16[c][current_bitfield], current_bit) ){
               framebuffer_device_draw_pixel(fb,
                       curr_x,
                       curr_y,
                       &(graphics_pixel){.Red = 0xff, .Green = 0xff, .Blue = 0xff, .Alpha = 0xff}
                );
               framebuffer_device_draw_pixel(fb,
                       curr_x+1,
                       curr_y,
                       &(graphics_pixel){.Red = 0xff, .Green = 0xff, .Blue = 0xff, .Alpha = 0xff}
                );
               framebuffer_device_draw_pixel(fb,
                       curr_x,
                       curr_y+1,
                       &(graphics_pixel){.Red = 0xff, .Green = 0xff, .Blue = 0xff, .Alpha = 0xff}
                );
               framebuffer_device_draw_pixel(fb,
                       curr_x+1,
                       curr_y+1,
                       &(graphics_pixel){.Red = 0xff, .Green = 0xff, .Blue = 0xff, .Alpha = 0xff}
                );

           }
           curr_x += 2;
        }
        curr_y += 2;
    }
}
