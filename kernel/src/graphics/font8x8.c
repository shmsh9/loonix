#include <graphics/font8x8.h>
char ** font8x8_new(){
    #include <graphics/font8x8_basic.h>
    //char font8x8_basic[128][8]
    char **sprite_array = kcalloc(128,sizeof(char *));
    for(uint8_t i = 0; i < 128; i++){
        sprite_array[i] = kcalloc(8, sizeof(char));
        memcpy(sprite_array[i], font8x8_basic[i], sizeof(char)*8);
    }
    return sprite_array;
}
void font8x8_free(char **font){
    for(uint8_t i = 0; i < 128; i++){
        kfree(font[i]);
    }
    kfree(font);
}
void font8x8_draw_framebuffer(framebuffer_device *fb, uint64_t x, uint64_t y, uint8_t c){
    switch((uintptr_t)font8x8){
        case 0x0:
            //font not initialized
            return;
        default:
            break;
    }
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
               curr_x++;
            }
            curr_y++;
        }
            return;
    }
    uint64_t curr_y = y;
    uint64_t curr_x = x;
    for(uint8_t current_bitfield = 0; current_bitfield < 8; current_bitfield++){
        curr_x = x;
        for(uint8_t current_bit = 0; current_bit < 8; current_bit++){
           if((font8x8[c][current_bitfield] >> current_bit) & 1){
               framebuffer_device_draw_pixel(fb,
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

