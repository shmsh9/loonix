#include <drivers/vt100.h>

uint64_t vt100_console_current_x = 0;
uint64_t vt100_console_current_y = 0;
uint8_t vt100_console_previous_char = 0;
uint64_t vt100_console_escaping_value = 0;
uint8_t vt100_console_escaping_char = 0;
bool vt100_console_escaping = false;
void vt100_console_increase_x(framebuffer_device *fb){
    vt100_console_current_x = vt100_console_current_x+8 < fb->width ? vt100_console_current_x+8 : 0;
}
void vt100_console_increase_y(framebuffer_device *fb){
    vt100_console_current_y = vt100_console_current_y+8+2 < fb->height ? vt100_console_current_y+8+2 : 2;

}
void vt100_console_reset_x(framebuffer_device *fb){
    vt100_console_current_x = 0;
}
void vt100_console_reset_y(framebuffer_device *fb){
    vt100_console_current_y = 0;
}
void vt100_console_escaping_stop(framebuffer_device *fb, uint8_t c){
    vt100_console_escaping = !vt100_console_escaping;
    switch(c){
        case 'D':
            vt100_console_current_x -= vt100_console_escaping_value;
            break;
    }
    vt100_console_escaping_value = 0;
}
void vt100_console_putchar(framebuffer_device *fb, uint8_t c){
    switch((uintptr_t)fb->buffer){
        case 0x0:
            break;
        default:
            if(vt100_console_escaping){
                    switch(c){
                        case '\33':
                            vt100_console_escaping_stop(fb, c);
                            break;
                        case 'C':
                            vt100_console_escaping_stop(fb, c);
                            break;
                        case 'D':
                            vt100_console_escaping_stop(fb, c);
                            break;
                        default:{
                            char tmp_char[2] = {c, 0};
                            int tmp_result = atoi(tmp_char);
                            vt100_console_escaping_value += tmp_result == -1 ? 0 : tmp_result;
                            vt100_console_escaping_value *= 10;
                            break;
                        }
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
                    //VT100 escape code
                    case '\33':
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
    vt100_console_previous_char = c;

}
