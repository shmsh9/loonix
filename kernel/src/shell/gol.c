#include <shell/gol.h>

int gol_entropy(void *data, task *t){
    int i = 3;
    while(1){
        i *=i;
        if(i > 1)
            i /= i-1;
        else
            i++;
    }
}
void gol_print(karray *game, int cur_x, int cur_y){
    graphics_pixel b_less_pixel[] = {
        GRAPHICS_PIXEL_BLACK,
        GRAPHICS_PIXEL_GREEN
    };
    //framebuffer_device_clear(fb, &GRAPHICS_PIXEL_BLACK);
    for(int i = 0; i < game->length; i++){
        for(int j = 0; j < fb->width; j++){
            framebuffer_device_draw_pixel_fast(
                fb,
                j,
                i,
                &b_less_pixel[(uint8_t)((char **)game->array)[i][j]]
            );
            
        }
    }
    framebuffer_device_draw_pixel_fast(
        fb,
        cur_x,
        cur_y,
        &GRAPHICS_PIXEL_RED
    );
    framebuffer_device_update(fb);
}
void gol_tick(karray *game, uint8_t rand){
    for(int i = 0; i < game->length; i++){
        for(int j = 0; j < fb->width; j++){
            int neigh = 0;
            if(j - 1 >= 0){
                neigh += ((char **)game->array)[i][j-1];
            }
            if(j + 1 < fb->width){
                neigh += ((char **)game->array)[i][j+1];
            }
            if(i -1 >= 0){
                int start = j - 1 >= 0 ? j - 1 : j;
                int end = j + 1 < fb->width ? j + 1 : j;
                for(int x = start; x < end; x++){
                    neigh += ((char **)game->array)[i-1][x];
                }
            }
            if(i+1 < game->length){
                int start = j - 1 >= 0 ? j - 1 : j;
                int end = j + 1 < fb->width ? j + 1 : j;
                for(int x = start; x < end; x++){
                    neigh += ((char **)game->array)[i+1][x];
                }

            }
            if(neigh > 8){
                KPANIC("NEIGH > 8");
            }
                                  //neigh == 0, 1, 2, 3, 4, 5, 6, 7, 8
            uint8_t b_less_choice_dead[]  = { 0, 0, 0, 1, 0, 0, 0, 0, 0 };
            uint8_t b_less_choice_alive[] = { 0, 0, 1, 1, 0, 0, 0, 0, 0 };
            uint8_t *b_less_choice[] = {b_less_choice_dead, b_less_choice_alive};
            ((char **)game->array)[i][j] = b_less_choice[(uint8_t)((char **)game->array)[i][j]][neigh];
            if((cpu_get_tick() & 0xaa) == 0){
                switch(rand){
                    case 0:
                        break;
                    case 1:
                        ((char **)game->array)[i][j] = 1;
                        break;
                    case 2:
                        ((char **)game->array)[i][j] = 0;
                        break;
                }
            }
        }
    }
}
int builtins_gol(int argc, char **argv){
    kprintf("Lame Game Of Life :\nWASD to move\nSpace do spawn cell\nP to start/stop generation\nR to generate random cells\nX to kill random cells\n");
    framebuffer_device_update(fb);
    char c = kgetchar();
    c++;
    task_new(
        gol_entropy,
        0x0,
        "gol_entropy",
        task_priority_low
    );
    karray *game = karray_new(sizeof(char *), kfree);
    for(int i = 0; i < fb->height; i++){
        karray_push(game, (uint64_t)kcalloc(fb->width,1));
    }
    int curr_x = fb->width/2;
    int curr_y = fb->height/2;
    bool pause = true; 
    while(!ps2_key_is_pressed(PS2_KEY_ESCAPE)){
        uint8_t rand = 0;
        if(ps2_key_is_pressed(PS2_KEY_R)){
            rand = 1;
        }
        if(ps2_key_is_pressed(PS2_KEY_X)){
            rand = 2;
        }
        if(ps2_key_is_pressed(PS2_KEY_W)){
            curr_y--;
        }
        if(ps2_key_is_pressed(PS2_KEY_A)){
            curr_x--;
        }
        if(ps2_key_is_pressed(PS2_KEY_S)){
            curr_y++;
        }
        if(ps2_key_is_pressed(PS2_KEY_D)){
            curr_x++;
        }
        if(ps2_key_is_pressed(PS2_KEY_SPACE)){
            ((char **)game->array)[curr_y][curr_x] = 1;
        }
        if(ps2_key_is_pressed(PS2_KEY_P)){
            pause = !pause;
        }
        gol_print(game, curr_x, curr_y);
        if(!pause)
            gol_tick(game, rand);
    }
    karray_free(game);
    return 0;
}
