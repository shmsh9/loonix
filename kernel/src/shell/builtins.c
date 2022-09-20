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
    kprintf("argc == %d\n", argc);
    return argc;
}
int builtins_time(int argc, char **argv){
    uint64_t t1  = cpu_get_tick();
    if(argc >= 2)
        shell_exec(argv[1]);
    uint64_t t2 = cpu_get_tick();
    kprintf("%d\n", t2-t1);
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

int builtins_teststrdup(int argc, char **argv){
    char **f = kcalloc(sizeof(char *), 0xfff);
    kprintf("f == 0x%x\n", f);
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
    uint32_t tux_x = fb->width/2 - tux->width/2;
    uint32_t tux_y = fb->height/2 - tux->height/2;
    while(1){
        ps2_device_getchar_non_blocking(ps2);
        char serial_char = serial_device_readchar_non_blocking(serial);
        //ps2_keypress_update(k);
        if(ps2_key_is_pressed(PS2_KEY_ESCAPE) || serial_char == 0x1b){
            KMESSAGE("Exit");
            break;
        }
        if(ps2_key_is_pressed(PS2_KEY_W) || tolower(serial_char) == 'w'){
            if(tux_y - 1 > 0)
                tux_y--;
        }
        if(ps2_key_is_pressed(PS2_KEY_A) ||  tolower(serial_char) == 'a'){
            tux_x--;
        }
        if(ps2_key_is_pressed(PS2_KEY_S) || tolower(serial_char) == 's'){
            if(tux->height+tux_y+1 < fb->height)
                tux_y++;
        }
        if(ps2_key_is_pressed(PS2_KEY_D) ||  tolower(serial_char) == 'd'){
            tux_x++;
        }

	    framebuffer_device_clear(fb, 
            &(graphics_pixel){
                .Red = 0xaf,
                .Green = 0xaf,
                .Blue = 0xaf
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
int builtins_exit(int argc, char **argv){
    event_loop_remove_by_function(shell_non_blocking);
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
    BUILTINS_INIT_FN(builtins_clear, "clear");
    BUILTINS_INIT_FN(builtins_free, "free");
    BUILTINS_INIT_FN(builtins_int, "int");
    BUILTINS_INIT_FN(builtins_time, "time");
    BUILTINS_INIT_FN(builtins_exit, "exit");
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

