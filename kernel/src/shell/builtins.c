#include <shell/builtins.h>

struct fnbuiltins builtins = {0};

int clear(int argc, char **argv){
    kprint("\033[2J\033[H");
    framebuffer_clear(&fb, &(graphics_pixel){.Red = 0x00, .Green = 0x00, .Blue = 0x00, .Alpha = 0xFF});
    framebuffer_text_current_x = 0; 
    framebuffer_text_current_y = 0;
    return 0;
}
int help(int argc, char **argv){
    for(int i = 0 ; i < builtins.length; i++){
        kprint(builtins.builtins[i].name);
        kputc('\n');
    }
    return 0;
}
