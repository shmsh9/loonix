#include <shell/gol.h>

int gol_entropy(void *data, task *t){
    uint8_t rnd = (uint64_t)data;
    int i = 3302283 ^ rnd;
    while(1){
        i *= rnd ^ i;
        if(i > 1)
            i /= i-1;
        else
            i++;
    }
}
void gol_print(uint8_t *cells, int cur_x, int cur_y){
    graphics_pixel b_less_pixel[] = {
        GRAPHICS_PIXEL_BLACK,
        GRAPHICS_PIXEL_WHITE
    };

    uint32_t y = 0;
    uint32_t x = 0;
    uint32_t l = fb->width*fb->height;

    for(int i = 0; i < l; i++){
        framebuffer_device_draw_pixel_fast(
                fb,
                x,
                y,
                &b_less_pixel[cells[i]]
            );

        x++;
        if(x == fb->width){
            y++;
            x = 0;
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
void gol_tick(uint8_t *cells, uint8_t rand){

    

    uint8_t b_less_choice[2][8] = {
        //neigh == 0, 1, 2, 3, 4, 5, 6, 7
        { 0, 0, 0, 1, 0, 0, 0, 0 }, //dead
        { 0, 0, 1, 1, 0, 0, 0, 0 }  //alive
    };

    uint32_t x = 0;
    uint32_t y = 0;

    uint32_t w = fb->width;
    uint32_t h = fb->height;
    uint32_t l = w*h;

    for(int i = 0; i < l; i++){

        uint8_t neigh = 0;
        //upper neigh
        if(y > 0){
            if(x == 0)
                neigh += cells[i-w+1] + cells[i-w];
            else if (x == w-1)
                neigh += cells[i-w-1] + cells[i-w];
            else
                neigh += cells[i-w-1] + cells[i-w] + cells[i-w+1];
        }

        //inline neigh
        if(x == 0)
            neigh += cells[i+1];
        else if (x == w-1)
            neigh += cells[i-1];
        else
            neigh += cells[i-1] + cells[i+1];

        //down neigh
        if(y+1 < h){
            if(x == 0)
                neigh += cells[i+w] + cells[i+w+1];
            else if (x == w-1)
                neigh += cells[i+w] + cells[i+w-1];
            else
                neigh += cells[i+w-1] + cells[i+w] + cells[i+w+1];
        }

        if(neigh > 8 || neigh < 0)
            KERROR("neigh fucked up");
        cells[i] = b_less_choice[cells[i]][neigh];
        if((cpu_get_tick() & 0xaa) == 0){
            switch(rand){
                case 0:
                    break;
                case 1:
                    cells[i] = 1;
                    break;
                case 2:
                    cells[i] = 0;
                    break;
            }
        }

        x++;
        if(x == fb->width){
            y++;
            x = 0;
        }    
    }
}
int builtins_gol(int argc, char **argv){
    kprintf("Lame Game Of Life :\nWASD to move\nSpace do spawn cell\nP to start/stop generation\nR to generate random cells\nX to kill random cells\n");
    char c = kgetchar();
    /*
    task *entropy = task_new(
        gol_entropy,
        (void *)(uint64_t)c,
        "gol_entropy",
        task_priority_low
    );
    */
    c++;
    uint8_t *cells = kcalloc(fb->height*fb->width, 1);

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
            if(curr_y -1 >= 0)
                curr_y--;
        }
        if(ps2_key_is_pressed(PS2_KEY_A)){
            if(curr_x -1 >= 0)
                curr_x--;
        }
        if(ps2_key_is_pressed(PS2_KEY_S)){
            if(curr_y + 1 < fb->height)
                curr_y++;
        }
        if(ps2_key_is_pressed(PS2_KEY_D)){
            if(curr_x +1 < fb->width)
                curr_x++;
        }
        if(ps2_key_is_pressed(PS2_KEY_SPACE)){
            cells[curr_x+(fb->width*curr_y)] = 1;
            //((char **)game->array)[curr_y][curr_x] = 1;
        }
        if(ps2_key_is_pressed(PS2_KEY_P)){
            pause = !pause;
        }
        gol_print(cells, curr_x, curr_y);
        if(!pause)
            gol_tick(cells, rand);
    }
    kfree(cells);
    //task_end(entropy);
    return 0;
}
