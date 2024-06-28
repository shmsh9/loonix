#include <drivers/vt100.h>

uint64_t vt100_console_escaping_value = 0;
uint8_t  vt100_console_font_width = 8;
uint8_t  vt100_console_font_height = 8;
uint8_t  vt100_console_font_y_spacing = 4;
uint8_t  vt100_console_font_x_spacing = 0;
uint64_t vt100_console_current_x = 0;
uint64_t vt100_console_current_y = 2;
uint64_t vt100_console_tab_size = 9;
uint64_t vt100_console_last_draw_timer = 0;
uint64_t vt100_console_last_draw_offset = 0;
bool vt100_console_escaping = false;
bool vt100_console_initialized = false;
bool vt100_console_cursor_enabled = true;
bool vt100_console_cursor_drawed = false;
bitmap_font vt100_console_bitmap_font = {0};
uint8_t vt100_cursor_state = VT100_CURSOR_OFF;
uint8_t vt100_cursor_current_char = ' ';

void (*vt100_console_font_drawing_function)(char **, framebuffer_device *,uint64_t, uint64_t, uint8_t);
void vt100_set_cursor_char(uint8_t c){
    vt100_cursor_current_char = c;
}
void vt100_console_init(framebuffer_device *fb){
    vt100_console_bitmap_font = bitmap_font_new();
    if(fb->width >= 1920){
        vt100_console_font_drawing_function = bitmap_font_16x16_draw_framebuffer;
        vt100_console_font_width = 16;
        vt100_console_font_height = 16;
        vt100_console_font_y_spacing = 4;
        vt100_console_font_x_spacing = 0;
    }
    else{
        vt100_console_font_drawing_function = bitmap_font_8x8_draw_framebuffer;
        vt100_console_font_width = 8;
        vt100_console_font_height = 8;
        vt100_console_font_y_spacing = 4;
        vt100_console_font_x_spacing = 0;
    }
    vt100_console_initialized = true;
}
void vt100_console_update_draw_screen(framebuffer_device *fb){
    if(cpu_get_tick() >= vt100_console_last_draw_timer+VT100_REFRESH_TICK){
        //framebuffer_device_update_partial(fb, vt100_console_last_draw_offset, fb->size - vt100_console_last_draw_offset);
        if(vt100_console_cursor_enabled){
            if(vt100_cursor_state >= VT100_CURSOR_FIRST_FRAME){
                if(!vt100_console_cursor_drawed){
                    vt100_console_font_drawing_function(
                        vt100_console_bitmap_font,
                        fb,
                        vt100_console_current_x-vt100_console_font_width,
                        vt100_console_current_y,
                        0x0
                    );
                    vt100_console_cursor_drawed = true;
                }
                if(vt100_cursor_state == VT100_CURSOR_LAST_FRAME){
                    vt100_console_font_drawing_function(
                        vt100_console_bitmap_font,
                        fb,
                        vt100_console_current_x-vt100_console_font_width,
                        vt100_console_current_y,
                        vt100_cursor_current_char
                    );
                    vt100_cursor_state = VT100_CURSOR_OFF;
                    vt100_console_cursor_drawed = false;
                }
            }
            vt100_cursor_state++;
        }
        framebuffer_device_update(fb);
        vt100_console_last_draw_offset = vt100_console_current_x*vt100_console_current_y;
        vt100_console_last_draw_timer = cpu_get_tick();
    }
}
void vt100_console_increase_x(framebuffer_device *fb){
    vt100_console_set_x(
        fb, 
        vt100_console_current_x+vt100_console_font_width+vt100_console_font_x_spacing
    );
}
void vt100_console_increase_y(framebuffer_device *fb){
    vt100_console_set_y(
        fb, 
        vt100_console_current_y+vt100_console_font_height+vt100_console_font_y_spacing
    );
}
void vt100_console_decrease_x(framebuffer_device *fb){
    vt100_console_set_x(
        fb,
        vt100_console_current_x - (vt100_console_font_width+vt100_console_font_x_spacing)
    );
}
void vt100_console_reset_x(framebuffer_device *fb){
    vt100_console_current_x = vt100_console_font_x_spacing;
}
void vt100_console_reset_y(framebuffer_device *fb){
    vt100_console_current_y = vt100_console_font_y_spacing;
}
void vt100_console_set_x(framebuffer_device *fb, uint64_t x){
    if(x >= fb->width){
        vt100_console_current_x = vt100_console_font_x_spacing;
        vt100_console_increase_y(fb);
    }
    else{
        vt100_console_current_x = x;
    }
}
void vt100_console_set_y(framebuffer_device *fb, uint64_t y){
    if(y >= fb->height){
        //need to implement framebuffer scroll;
        framebuffer_device_scroll_down(fb, 
        1*(vt100_console_font_height+vt100_console_font_y_spacing)
        );
    }
    else{
        vt100_console_current_y = y;
    }

}
void vt100_console_escaping_stop(framebuffer_device *fb, uint8_t c){ 
    switch(c){
        case '[':
            return;
            break;
        case 'D':
            vt100_console_set_x(fb, 
            vt100_console_current_x-(vt100_console_escaping_value*(vt100_console_font_width+vt100_console_font_x_spacing)));
            break;
        case 'C':
            vt100_console_set_x(fb, 
            vt100_console_current_x+(vt100_console_escaping_value*(vt100_console_font_width+vt100_console_font_x_spacing)));
            break;
        case 'H':
            vt100_console_reset_x(fb);
            vt100_console_reset_y(fb);
            break;
        case 'J':
            framebuffer_device_clear(fb ,&(graphics_pixel){.Red = 0x0, .Green = 0x0, .Blue = 0x0, .Alpha = 0xff});
            break;
    }
    vt100_console_escaping = !vt100_console_escaping;
    vt100_console_escaping_value = 0;
}
void vt100_console_putchar(framebuffer_device *fb, uint8_t c){
    if(!vt100_console_initialized)
        return;
    switch((uintptr_t)fb->buffer){
        case 0x0:
            break;
        default:
            if(vt100_console_cursor_drawed){
                vt100_console_font_drawing_function(
                    vt100_console_bitmap_font,
                    fb,
                    vt100_console_current_x-vt100_console_font_width,
                    vt100_console_current_y,
                    vt100_cursor_current_char
                );

            }
            vt100_cursor_state = VT100_CURSOR_OFF;
            vt100_console_cursor_drawed = false;
            if(vt100_console_escaping){
                int tmp_result = isdigit(c);
                switch(tmp_result){
                    case 0:
                        vt100_console_escaping_stop(fb, c);
                        break;
                    default:
                        vt100_console_escaping_value *= 10;
                        vt100_console_escaping_value += tmp_result;
                        break;
                }
            }
            else{
                switch (c){
                    case '\n':
                        vt100_console_reset_x(fb);
                        vt100_console_increase_y(fb);
                        break;
                    case '\r':
                        vt100_console_reset_x(fb);
                        break;
                    case '\b':
                        vt100_console_decrease_x(fb);
                        break;
                    case '\t':
                        kprintf("\033[%dC", vt100_console_tab_size);
                        break;
                    //VT100 escape code
                    case '\033':
                        vt100_console_escaping = !vt100_console_escaping;
                        break;
                    default:
                        vt100_console_font_drawing_function(
                            vt100_console_bitmap_font,
                            fb,
                            vt100_console_current_x,
                            vt100_console_current_y,
                            c
                        );
                        vt100_console_increase_x(fb);
                        break;
                }
                break;
            }
    }
    vt100_console_update_draw_screen(fb);
}
