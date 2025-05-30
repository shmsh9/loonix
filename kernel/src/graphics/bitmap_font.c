#include <graphics/bitmap_font.h>

bitmap_font bitmap_font_new(){
    #include <graphics/font8x8_basic.h>
    //char font8x8_basic[128][8]
    bitmap_font sprite_array = kcalloc(128,sizeof(char *));
    for(uint8_t i = 0; i < 128; i++){
        sprite_array[i] = kcalloc(8, sizeof(char));
        memcpy(sprite_array[i], font8x8_basic[i], sizeof(char)*8);
    }
    return sprite_array;
}
void bitmap_font_free(bitmap_font font){
    for(uint8_t i = 0; i < 128; i++){
        kfree(font[i]);
    }
    kfree(font);
    font = NULL;
}
void bitmap_font_16x16_draw_color_framebuffer(bitmap_font font16x16, framebuffer_device *fb, uint64_t x, uint64_t y, uint8_t c, graphics_pixel fg, graphics_pixel bg){
    if(!font16x16)
        return;
    if(c >= 128 || c < 0){
        KERROR("c is out of ascii range");
        return;
    }
    graphics_pixel *px_branchless[2] = {
        &bg,
        &fg
    };
    uint64_t curr_y = y;
    uint64_t curr_x = x;
    for(uint8_t current_bitfield = 0; current_bitfield < 8; current_bitfield++){
        curr_x = x;
        for(uint8_t current_bit = 0; current_bit < 8; current_bit++){
               framebuffer_device_draw_pixel_fast(fb,
                       curr_x,
                       curr_y,
                       px_branchless[ (font16x16[c][current_bitfield] >> current_bit) & 1]
                );
               framebuffer_device_draw_pixel_fast(fb,
                       curr_x+1,
                       curr_y,
                       px_branchless[ (font16x16[c][current_bitfield] >> current_bit) & 1]
                );
               framebuffer_device_draw_pixel_fast(fb,
                       curr_x,
                       curr_y+1,
                       px_branchless[ (font16x16[c][current_bitfield] >> current_bit) & 1]
                );
               framebuffer_device_draw_pixel_fast(fb,
                       curr_x+1,
                       curr_y+1,
                       px_branchless[ (font16x16[c][current_bitfield] >> current_bit) & 1]
                );
           curr_x += 2;
        }
        curr_y += 2;
    }

}
void bitmap_font_8x8_draw_color_framebuffer(bitmap_font font8x8, framebuffer_device *fb, uint64_t x, uint64_t y, uint8_t c, graphics_pixel fg, graphics_pixel bg){
    if(!font8x8)
        return;
    if(c >= 128 || c < 0){
        KERROR("c is out of ascii range");
        return;
    }
    graphics_pixel *px_branchless[2] = {
        &bg,
        &fg
    };
    uint64_t curr_y = y;
    uint64_t curr_x = x;
    for(uint8_t current_bitfield = 0; current_bitfield < 8; current_bitfield++){
        //curr_x = x;
        framebuffer_device_draw_pixel_fast(
            fb,
            curr_x,
            curr_y,
            px_branchless[ (font8x8[c][current_bitfield] >> 0) & 1]

        );
        framebuffer_device_draw_pixel_fast(
            fb,
            curr_x+1,
            curr_y,
            px_branchless[ (font8x8[c][current_bitfield] >> 1) & 1]

        );
        framebuffer_device_draw_pixel_fast(
            fb,
            curr_x+2,
            curr_y,
            px_branchless[ (font8x8[c][current_bitfield] >> 2) & 1]

        );
        framebuffer_device_draw_pixel_fast(
            fb,
            curr_x+3,
            curr_y,
            px_branchless[ (font8x8[c][current_bitfield] >> 3) & 1]

        );
        framebuffer_device_draw_pixel_fast(
            fb,
            curr_x+4,
            curr_y,
            px_branchless[ (font8x8[c][current_bitfield] >> 4) & 1]

        );
        framebuffer_device_draw_pixel_fast(
            fb,
            curr_x+5,
            curr_y,
            px_branchless[ (font8x8[c][current_bitfield] >> 5) & 1]

        );
        framebuffer_device_draw_pixel_fast(
            fb,
            curr_x+6,
            curr_y,
            px_branchless[ (font8x8[c][current_bitfield] >> 6) & 1]

        );
        framebuffer_device_draw_pixel_fast(
            fb,
            curr_x+7,
            curr_y,
            px_branchless[ (font8x8[c][current_bitfield] >> 7) & 1]

        );
        curr_y++;
    }
}

void bitmap_font_16x16_draw_framebuffer(bitmap_font font16x16, framebuffer_device *fb, uint64_t x, uint64_t y, uint8_t c){
	bitmap_font_16x16_draw_color_framebuffer(font16x16, fb, x, y, c, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_BLACK);
}
void bitmap_font_8x8_draw_framebuffer(bitmap_font font8x8, framebuffer_device *fb, uint64_t x, uint64_t y, uint8_t c){
	bitmap_font_8x8_draw_color_framebuffer(font8x8, fb, x, y, c, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_BLACK);
}
