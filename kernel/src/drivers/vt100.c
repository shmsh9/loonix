#include <drivers/vt100.h>

uint64_t vt100_console_escaping_value = 0;
uint8_t  vt100_console_font_size = 8;
uint8_t  vt100_conole_font_y_spacing = 2;
uint64_t vt100_console_current_x = 0;
uint64_t vt100_console_current_y = 2;

bool vt100_console_escaping = false;

void vt100_console_increase_x(framebuffer_device *fb){
    vt100_console_set_x(fb, vt100_console_current_x+vt100_console_font_size);

}
void vt100_console_increase_y(framebuffer_device *fb){
    vt100_console_set_y(fb, 
    vt100_console_current_y+vt100_console_font_size+vt100_conole_font_y_spacing);
}
void vt100_console_decrease_x(framebuffer_device *fb){
    vt100_console_set_x(fb,
        vt100_console_current_x - vt100_console_font_size
    );
}
void vt100_console_reset_x(framebuffer_device *fb){
    vt100_console_current_x = 0;
}
void vt100_console_reset_y(framebuffer_device *fb){
    vt100_console_current_y = vt100_conole_font_y_spacing;
}
void vt100_console_set_x(framebuffer_device *fb, uint64_t x){
    if(x >= fb->width){
        vt100_console_current_x = 0;
        vt100_console_increase_y(fb);
    }
    else{
        vt100_console_current_x = x;
    }
}
void vt100_console_set_y(framebuffer_device *fb, uint64_t y){
    if(y >= fb->height){
        //need to implement framebuffer scroll;
        vt100_console_current_y = 0;
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
            vt100_console_set_x(fb, vt100_console_current_x-(vt100_console_escaping_value*vt100_console_font_size));
            break;
        case 'C':
            vt100_console_set_x(fb, vt100_console_current_x+(vt100_console_escaping_value*vt100_console_font_size));
            break;
        case 'H':
            vt100_console_reset_x(fb);
            vt100_console_reset_y(fb);
            break;
        case 'J':
            framebuffer_clear(fb ,&(graphics_pixel){.Red = 0x0, .Green = 0x0, .Blue = 0x0, .Alpha = 0xff});
            break;
    }
    vt100_console_escaping = !vt100_console_escaping;
    vt100_console_escaping_value = 0;
}
void vt100_console_putchar(framebuffer_device *fb, uint8_t c){
    switch((uintptr_t)fb->buffer){
        case 0x0:
            break;
        default:
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
                break;
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
                    //VT100 escape code
                    case '\033':
                        vt100_console_escaping = !vt100_console_escaping;
                        break;
                    default:
                        font8x8_draw_framebuffer(fb,
                            vt100_console_current_x,
                            vt100_console_current_y,
                            c
                        );
                        framebuffer_update_device(fb);
                        vt100_console_increase_x(fb);
                        break;
                }
                break;
            }
    }
}
