#include <shell/builtins.h>

struct fnbuiltins builtins = {0};

int builtins_clear(int argc, char **argv){
    kprint("\033[2J\033[H");
    return 0;
}
int builtins_help(int argc, char **argv){
    for(int i = 0 ; i < builtins.length; i++){
        kprintf("%s\n", builtins.builtins[i].name);
    }
    return 0;
}
int builtins_testargs(int argc, char **argv){
    for(int i = 0; i < argc; i++){
        kprintf("argv[%d] == %s\n", (uint64_t)i, argv[i]);
    }
    return argc;
}
int builtins_time(int argc, char **argv){
    uint64_t t1  = cpu_get_tick();
    uint64_t s1 = getuptime100s();
    if(argc >= 2)
        shell_exec(argv[1]);
    uint64_t t2 = cpu_get_tick();
    uint64_t s2 = getuptime100s();
    kprintf("\nticks   \t%d\nseconds \t%d\n", t2-t1, (s2-s1)/100);
    return 0;
}
int builtins_testkarray(int argc, char **argv){
    karray *k = karray_new(sizeof(uint64_t),NULL);
    for(uint64_t i = 0; i < 0xff; i++)
        karray_push(k, i);
    karray_debug_print(k);
    karray_free(k);
    return 0;
}
int builtins_testklist(int argc, char **argv){
    klist *k = klist_new(kfree);
    for(uint64_t i = 0; i < 0xff; i++){
        klist_push(k,(uintptr_t)strdup("foobar"));
    }
    klist_debug_print(k);
    klist_free(k);
    return 0;
}
int builtins_testkcalloc(int argc, char **argv){
    #define TEST_KCALLOC_SZ 512
    void *arr[TEST_KCALLOC_SZ] = {0};
    for(int i = 0; i < TEST_KCALLOC_SZ; i++){
        arr[i] = kcalloc(8,1);
    }
    for(int i = 0; i < TEST_KCALLOC_SZ; i++){
        kfree(arr[i]);
    }
    return 0;
}
int builtins_uptime(int argc, char **argv){
    kprintf("up since %d seconds\n", getuptime100s()/100);
    return 0;
}
int builtins_teststrdup(int argc, char **argv){
    char **f = kcalloc(sizeof(char *), 0xfff);
    karray *k = karray_new(sizeof(uintptr_t), kfree);
    for(int i = 0; i < 0xfff; i++){
        karray_push(k, (uintptr_t)strdup("1234"));
        f[i] = strdup("1234");
    }
    karray_free(k);
    for(int i = 0; i < 0xfff; i++){
        kfree(f[i]);
    }

    kfree(f);
    return 0;
}

int builtins_testscroll(int argc, char **argv){
    kprint("{ ");
    for(int i = 0; i < 0xffff; i++){
        kprintf("0x%x", i);
        if(i+1 < 0xffff)
            kprint(", ");
    }
    kprint("}\n");
    return 0;
}
int builtins_testmemset(int argc, char **argv){
    char foo[17] = {0};
    //char foo2 = {'0', '1', '2', '3', '4', '5', '6', '7'};
    kprintf("foo = %s\n", foo);
    //cpu_registers r = {0};
    //cpu_registers_save(&r);
    //kprintf("regs before :\n");
    //CPU_REGISTERS_PRINT(&r);
    __memset_64b(foo, B_to_8B('X'), 16);
    //cpu_registers_save(&r);
    //kprintf("regs after :\n");
    //CPU_REGISTERS_PRINT(&r);
    kprintf("foo = %s\n", foo);
    return 0;
}
int builtins_testmemcpy(int argc, char **argv){
    char foo[17] = {0};
    char foo2[] = {'0', '1', '2', '3', '4', '5', '6', '7', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    kprintf("foo = %s\n", foo);
    //cpu_registers r = {0};
    //cpu_registers_save(&r);
    //kprintf("regs before :\n");
    //CPU_REGISTERS_PRINT(&r);
    __memcpy_128b(foo, foo2, 16);
    //cpu_registers_save(&r);
    //kprintf("regs after :\n");
    //CPU_REGISTERS_PRINT(&r);
    kprintf("foo = %s\n", foo);
    return 0;
}

int builtins_graphics(int argc, char **argv){
	#include <graphics/tux.png.h>
	graphics_sprite *tux = graphics_sprite_static_new(216, 256, TUX_PIXELS);
    int32_t tux_x = fb->width/2 - tux->width/2;
    int32_t tux_y = fb->height/2 - tux->height/2;
    while(1){
        char serial_char = serial_device_readchar_non_blocking(serial);
        if(ps2_key_is_pressed(PS2_KEY_ESCAPE) || serial_char == 0x1b){
            KMESSAGE("Exit");
            break;
        }
        if(ps2_key_is_pressed(PS2_KEY_W) || tolower(serial_char) == 'w'){
                tux_y--;
        }
        if(ps2_key_is_pressed(PS2_KEY_A) || tolower(serial_char) == 'a'){
                tux_x--;
                //KMESSAGE("tux_x 0x%x && tux_y 0x%x", tux_x, tux_y);
        }
        if(ps2_key_is_pressed(PS2_KEY_S) || tolower(serial_char) == 's'){
                tux_y++;
        }
        if(ps2_key_is_pressed(PS2_KEY_D) || tolower(serial_char) == 'd'){
                tux_x++;
        }

	    framebuffer_device_clear(fb, 
            &(graphics_pixel){
                .Red = 0xbf,
                .Green = 0x01,
                .Blue = 0x9f
            }
        );
	    framebuffer_device_draw_sprite_slow(
            fb, 
            tux_x, 
            tux_y, 
            tux
        );
        framebuffer_device_update(fb);
    }
    graphics_sprite_static_free(tux);
    framebuffer_device_clear(
        fb,
        &(graphics_pixel){
            .Red = 0x00,
            .Green = 0x00,
            .Blue = 0x00,
            .Alpha = 0xff
        }
    );
    framebuffer_device_update(fb);
    return 0;
}

int builtins_free(int argc, char **argv){
    uint64_t total_mem = heap.n_block*HEAP_BLOCK_SIZE;
    uint64_t free_mem = heap.free_memory;
    uint64_t used_mem = total_mem - free_mem;
    kprintf("\ttotal\tused\tfree\nMem:\t%d\t%d\t%d\n", 
        total_mem, used_mem, free_mem
    );
    return 0;
}

int builtins_reboot(int argc, char **argv){
    runtime_services->ResetSystem(EfiResetCold, 0, 0, 0x0);
    return 0;
}

int builtins_poweroff(int argc, char **argv){
    runtime_services->ResetSystem(EfiResetShutdown, 0, 0, 0x0);
    return 0;
}

int builtins_int(int argc, char **argv){
    int f = 1;
    int z = 1;
    z--;
    f /= z;
    INTERRUPT();
    return 0;
}
int builtins_regdump(int argc, char **argv){
    cpu_registers r;
    cpu_registers_save(&r);
    CPU_REGISTERS_PRINT(&r);
    return 0;
}

int builtins_ahci(int argc, char **argv){
    ahci_controller *cont = ahci_controller_new();
    if(!cont)
        return -1;
    ahci_device *dev0 = ahci_device_new(cont, 0);
    if(!dev0){
        kfree(cont);
        return -1;
    }
    KDEBUG("ABAR at 0x%x", cont->abar >> 4);
    ahci_device_send_command(dev0, 0);
    ahci_device_free(dev0);
    ahci_controller_free(cont);
    return 0;
}
int builtins_karray_pop(int argc, char **argv){
    karray *k = karray_new(sizeof(uint64_t), NULL);
    karray_push(k, 1);
    karray_pop(k, 0);
    karray_debug_print(k);
    karray_free(k);
    return 0;

}
int builtins_atoi(int argc, char **argv){
    if(argc >= 2)
        KMESSAGE("%d", (uint64_t)atoi(argv[1]));
    return 0;
}
int timer_task(void *data, task *t){
	sleep((uint64_t)data);
	KMESSAGE("%ds passed", data);
    //remove 2 task close bug
    return 0;
}
int builtins_task(int argc, char **argv){
    task_debug_print();
    return 0;
}
int testtask(void *data, task *t){
    uint64_t *i = data;
    while (*i < 5){
        kprintf("i == %d        \r", *i);
        sleep(1);
    }
    //builtins_testscroll(0, 0x0);
    return 0;
}
int testtask2(void *data, task *t){
    char *f = kmalloc(512);
    f[0] = 'f';
    f[1] = 0;
    uint64_t *i = data;
    while(*i < 5){
        sleep(1);
        task_lock();
        *i += 1;
        task_unlock();
    }
    kfree(i);
    builtins_int(0, 0x0);
    return 0;
}

int builtins_testtask(int argc, char **argv){
    uint64_t *i = kmalloc(sizeof(uint64_t));
    *i = 0;
    task_new(testtask, (void *)i, "test", task_priority_high);
    task_new(testtask2, (void *)i, "test2", task_priority_high);
    task_new(
        (int (*)(void *, task *))builtins_graphics, 
        0x0, 
        "graphics", 
        task_priority_high
    );
    return 0;
}
int entropy_gol(void *data, task *t){
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
            if(((char **)game->array)[i][j]){
                if(neigh < 2 || neigh > 3){
                    ((char **)game->array)[i][j] = 0;
                }
            }
            else{
                if(neigh == 3)
                    ((char **)game->array)[i][j] = 1;
            }
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
int builtins_lspci(int argc, char **argv){
    for(int i = 0; i < pci_devices->length; i++){
        char *padding_bus, *padding_slot;
        uint8_t bus = ((pci_device **)(pci_devices->array))[i]->bus;
        uint8_t slot = ((pci_device **)(pci_devices->array))[i]->slot;
        uint8_t function = ((pci_device **)(pci_devices->array))[i]->function;
        uint16_t vendor = ((pci_device **)(pci_devices->array))[i]->header->vendor_id;
        uint16_t product = ((pci_device **)(pci_devices->array))[i]->header->device_id;
        uint16_t subclass = ((pci_device **)(pci_devices->array))[i]->header->subclass;

        padding_bus = "";
        if(bus < 10)
            padding_bus = "00";
        if(bus >= 10 && bus < 100)
            padding_bus = "0";
        
        padding_slot = slot > 10 ? "": "0";
        kprintf("%s%d:", padding_bus, bus);
        kprintf("%s%d.", padding_slot, slot);
        kprintf("%d", function);
        kprintf(" 0x%x & 0x%x ", vendor, product);
        kprintf("%s (0x%x)",
        pci_class_strings[((pci_device **)(pci_devices->array))[i]->header->class],
        subclass
        );
        kputc('\n');
    }
    return 0;
}
void builtins_init(){
    builtins.length = 0;
    BUILTINS_INIT_FN(builtins_help, "help");
    BUILTINS_INIT_FN(builtins_ahci, "ahci");
    BUILTINS_INIT_FN(builtins_lspci, "lspci");
    BUILTINS_INIT_FN(builtins_gol, "gol");
    BUILTINS_INIT_FN(builtins_clear, "clear");
    BUILTINS_INIT_FN(builtins_free, "free");
    BUILTINS_INIT_FN(builtins_int, "int");
    BUILTINS_INIT_FN(builtins_time, "time");
    BUILTINS_INIT_FN(builtins_atoi, "atoi");
    BUILTINS_INIT_FN(builtins_testtask, "testtask");
    BUILTINS_INIT_FN(builtins_task, "task");
    BUILTINS_INIT_FN(builtins_uptime, "uptime");
    BUILTINS_INIT_FN(builtins_regdump, "regdump");
    BUILTINS_INIT_FN(builtins_testargs, "testargs");
    BUILTINS_INIT_FN(builtins_testkarray, "testkarray");
    BUILTINS_INIT_FN(builtins_karray_pop, "testkarraypop");
    BUILTINS_INIT_FN(builtins_testklist, "testklist");
    BUILTINS_INIT_FN(builtins_testkcalloc, "testkcalloc");
    BUILTINS_INIT_FN(builtins_teststrdup, "teststrdup");
    BUILTINS_INIT_FN(builtins_testscroll, "testscroll");
    BUILTINS_INIT_FN(builtins_testmemset, "testmemset");
    BUILTINS_INIT_FN(builtins_testmemcpy, "testmemcpy");
    BUILTINS_INIT_FN(builtins_graphics, "graphics");
    BUILTINS_INIT_FN(builtins_reboot, "reboot");
    BUILTINS_INIT_FN(builtins_poweroff, "poweroff");

}

