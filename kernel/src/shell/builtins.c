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
int klisttask(void *data, task *t){
    klist *k = (klist *)data;
    for(uint64_t i = 0xff; i < 0xfff; i++){
        klist_push(k,(uintptr_t)strdup("foobar"));
    }
    return 0;
}
int builtins_testklist(int argc, char **argv){
    klist *k = klist_new(kfree);
    task *t = task_new(
        klisttask,
        (void *)k,
        "pushklist",
        task_priority_high
    );
    
    for(uint64_t i = 0; i < 0xff; i++){
        klist_push(k,(uintptr_t)strdup("foobar"));
    }
    task_end_wait(t);
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
    //karray_free(k);
    for(int i = 0; i < 0xfff; i++){
        //kfree(f[i]);
    }

    //kfree(f);
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
    ahci_controller_probe_ports(cont); 
    if(!cont)
        return -1;
    ahci_device *dev0 = ahci_device_new(cont, 0);
    if(!dev0){
        kfree(cont);
        return -1;
    }
    char buff[512] = {0};
    ahci_device_read(
        dev0,
        0,
        0,
        512,
        (uint64_t *)&buff
    );
    /*
    ahci_device_send_command(dev0, 0);
    ahci_device_free(dev0);
    ahci_controller_free(cont);
    */
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
    uint64_t *i = kmalloc(sizeof(uint64_t));
    *i = 0;
    while (*i < 20){
        kprintf("i == %d        \r", *i);
        *i += 1;
    }
    kfree(i);
    //builtins_testscroll(0, 0x0);
    return 0;
}
int builtins_testleak(int argc, char **argv){
    uint8_t *foo = kmalloc(512);
    foo++;
    return 0;
}
int testtask2(void *data, task *t){
    uint64_t *i = kmalloc(sizeof(uint64_t));
    *i = 0;
    while(*i < 20){
        *i += 1;
        sleep(1);
    }
    kfree(i);
    builtins_int(0, 0x0);
    return 0;
}
int builtins_testtask(int argc, char **argv){
    task_new(testtask, 0x0, "test", task_priority_medium);
    task_new(testtask2, 0x0, "test2", task_priority_medium);
    return 0;
}
int builtins_testkhash(int argc, char **argv){
    khashmap *h = khashmap_new();
    khashmap_set(h, "foobar", 0x1);
    khashmap_set(h, "foobarbar", 0x2);
    khashmap_set(h, "azerty", 0x3);
    khashmap_set(h, "[[[[[", 0x3);
    kprintf("h[azerty] == 0x%x\n", khashmap_get(h, "azerty"));
    kprintf("h[foobar] == 0x%x\n", khashmap_get(h, "foobar"));
    kprintf("h[Foobar] == 0x%x\n", khashmap_get(h, "Foobar"));
    khashmap_debug_print(h);
    khashmap_free(h);
    return 0;
}
int builtins_testktree(int argc, char **argv){
    ktree *t = ktree_new(5, 0);
    ktree_add(t, 1, 0);
    ktree_add(t, 2, 0);
    ktree_add(t, 3, 0);
    ktree_add(t, 4, 0);

    ktree_add(t, 6, 0);
    ktree_add(t, 7, 0);
    ktree_add(t, 8, 0);
    ktree_add(t, 9, 0x1337);

    ktree_debug_print(t, 0);
    kprint("ENDE \n");
    ktree_del(t, 5);
    ktree_del(t, 6);
    ktree_debug_print(t, 0);
    ktree_free(t);
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
    BUILTINS_INIT_FN(builtins_testleak, "testleak");
    BUILTINS_INIT_FN(builtins_task, "task");
    BUILTINS_INIT_FN(builtins_uptime, "uptime");
    BUILTINS_INIT_FN(builtins_regdump, "regdump");
    BUILTINS_INIT_FN(builtins_testargs, "testargs");
    BUILTINS_INIT_FN(builtins_testkarray, "testkarray");
    BUILTINS_INIT_FN(builtins_testktree, "testktree");
    BUILTINS_INIT_FN(builtins_testkhash, "testkhash");
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

