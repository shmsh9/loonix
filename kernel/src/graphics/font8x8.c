#include <graphics/font8x8.h>

void font8x8_draw_framebuffer(framebuffer_device *fb, uint64_t x, uint64_t y, uint8_t c){
    if(c >= 128){
        KERROR("c is out of ascii range");
        return;
    }
    #include <graphics/font8x8_basic.h>
    //char font8x8_basic[128][8]
    uint64_t curr_y = y;
    uint64_t curr_x = x;
    for(uint8_t current_bitfield = 0; current_bitfield < 8; current_bitfield++){
        curr_x = x;
        for(uint8_t current_bit = 0; current_bit < 8; current_bit++){
           if((font8x8_basic[c][current_bitfield] >> current_bit) & 1){
               framebuffer_draw_pixel(fb,
                       curr_x,
                       curr_y,
                       &(graphics_pixel){.Red = 0xff, .Green = 0xff, .Blue = 0xff, .Alpha = 0xff}
                       );
           }
           curr_x++;
        }
        curr_y++;
    }
}

