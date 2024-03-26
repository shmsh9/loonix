#include <shell/builtins.h>
#include <network/net.h>
#include <kstd/regex.h>
#include <kstd/cmp.h>

int builtins_clear(int argc, char **argv){
    kprint("\033[2J\033[H");
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
int builtins_get_exit_code(int argc, char **argv){
    int c = shell_get_exit_code();
    if(c < 0)
        kputc('-');
    kprintf("%d\n", (uint64_t)((~c)+1));
    return c;
}
int builtins_regex(int argc, char **argv){
    if(argc != 3){
        kprintf(
            "usage: %s expression string to match\n", 
            argv[0]
        );
        uint64_t t[][3] = {
            {(uint64_t)VM_UINT_ASSIGNMENT, (uint64_t)"  foobar = 100;", (uint64_t)true},
            {(uint64_t)VM_UINT_ASSIGNMENT, (uint64_t)"  foobar = abc;", (uint64_t)false},
            {(uint64_t)VM_STRING_ASSIGNMENT, (uint64_t)"  foobar = \"100\";", (uint64_t)true},
            {(uint64_t)VM_STRING_ASSIGNMENT, (uint64_t)"  foobar = 100\";", (uint64_t)false},
            {(uint64_t)VM_STRING_ASSIGNMENT, (uint64_t)"  foobar = \"abc\";", (uint64_t)true},
            {(uint64_t)VM_STRING_ASSIGNMENT, (uint64_t)"  foobar = 'abc';", (uint64_t)false},
            {(uint64_t)".*abc", (uint64_t)"  foobar = 'abc';", (uint64_t)false},
            {(uint64_t)".*abc", (uint64_t)"1234abc", (uint64_t)true},
            {(uint64_t)".*", (uint64_t)"1234abc", (uint64_t)true},
            {(uint64_t)"ab?", (uint64_t)"ab", (uint64_t)true},
            {(uint64_t)"ab?", (uint64_t)"a", (uint64_t)true},
            {(uint64_t)"ab?", (uint64_t)"ac", (uint64_t)false},
            {(uint64_t)"[a-z]{3}", (uint64_t)"abc", (uint64_t)true},
            {(uint64_t)"[a-z]{3}", (uint64_t)"1bc", (uint64_t)false},
            {(uint64_t)".{3}", (uint64_t)"1bc", (uint64_t)true},
            {(uint64_t)".{3}", (uint64_t)"1bcd", (uint64_t)false},
            {(uint64_t)"\\**", (uint64_t)"****", (uint64_t)true},
            {(uint64_t)"\\**", (uint64_t)"a", (uint64_t)false},
            {(uint64_t)VM_FN_DEFINITION, (uint64_t)"void * malloc(){ return NULL; }", (uint64_t)true},
            {(uint64_t)VM_FN_DEFINITION, (uint64_t)"int strlen () { return 0; }", (uint64_t)true},
            {(uint64_t)VM_FN_DEFINITION, (uint64_t)"int strlen(){return 1;}", (uint64_t)true},
            {(uint64_t)VM_FN_DEFINITION, (uint64_t)"int *strlen (){ return 3;}", (uint64_t)true},
            {(uint64_t)VM_FN_DEFINITION, (uint64_t)"int ** regex_automaton_new(){}", (uint64_t)true},
            {(uint64_t)VM_FN_DEFINITION, (uint64_t)"int ** __do_not_use(){}", (uint64_t)true},        
            {(uint64_t)VM_FN_DEFINITION, (uint64_t)"int *strlen()", (uint64_t)false},
            {(uint64_t)"[a-z,\\*,0]", (uint64_t)"a", (uint64_t)true},
            {(uint64_t)"[a-z,\\*,0]", (uint64_t)"ab", (uint64_t)false},
            {(uint64_t)"[a-z,\\s,\\*,0]", (uint64_t)"ab", (uint64_t)false},
            {(uint64_t)"[a-z,\\s,\\*,0]", (uint64_t)"a", (uint64_t)true},
            {(uint64_t)"[\\s,\\*,0,a-z]", (uint64_t)"a", (uint64_t)true},
            {(uint64_t)"[\\s,\\*,0,a-z]", (uint64_t)"x", (uint64_t)true},
        };
        for(int i = 0; i  < sizeof(t)/sizeof(t[0]); i++){
            char *rx = (char *)t[i][0];
            karray *r = regex_new(rx);
            char *s = (char *)(t[i][1]);
            bool exp = (bool)(t[i][2]);
            bool ok = regex_match(r,s) == exp;
            KDEBUG("regex_match(\"%s\", \"%s\") == %s\n\ttest %s",
                rx,
                s,
                exp ? "true" : "false",
                ok ? "Ok" : "Failed !!!"
            );
            if(!ok){
                for(int j = 0; j < r->length; j++)
                    regex_automaton_debug_print(((regex_automaton **)r->array)[j]);
            }
            karray_free(r);
        }
        #define RNEW(exp, s, r) {(uint64_t)exp, (uint64_t)s, (uint64_t)r, (uint64_t)_regex_static_new(exp)}
        uint64_t _static_t[][4] = {
            RNEW(VM_FN_DEFINITION, "int strlen(char *a, char *b){ return 0; }", true),
            RNEW(VM_FN_DEFINITION, "int 1strlen(char *a, char *b){ return 0; }", false),
            RNEW(VM_UINT_ASSIGNMENT, "  foobar = 100;", true),
            RNEW(VM_UINT_ASSIGNMENT, "  foobar = \"100;",false),
            RNEW(VM_STRING_ASSIGNMENT,"  foobar = \"100\";",true)

        };
        for(int i = 0; i  < sizeof(_static_t)/sizeof(_static_t[0]); i++){
            char *rx = (char *)_static_t[i][0];
            karray *r = (karray *)_static_t[i][3];
            char *s = (char *)(_static_t[i][1]);
            bool exp = (bool)(_static_t[i][2]);
            bool ok = regex_match(r,s) == exp;
            KDEBUG("_static_regex_match(\"%s\", \"%s\") == %s\n\ttest %s",
                rx,
                s,
                exp ? "true" : "false",
                ok ? "Ok" : "Failed !!!"
            );
            if(!ok){
                for(int j = 0; j < r->length; j++)
                    regex_automaton_debug_print(((regex_automaton **)r->array)[j]);
            }
        }
        char *expr = "a[b,c,d,e]";
        //const int start = _regex_static_bracket_start(expr);
        //const int stop = _regex_static_bracket_stop(expr);
        const int count = _regex_static_bracket_count(expr, _regex_static_bracket_start(expr));
        char test[] = _regex_static_bracket_comma(expr, _regex_static_bracket_start(expr), 4);
        kprintf("test[] = { ");
        for(int i = 0; i < sizeof(test)/sizeof(test[0]); i++)
            kprintf("'%c'%s", test[i], i+1 == sizeof(test)/sizeof(test[0]) ? " }\n" : ", " );
        kprintf("_regex_static_bracket_count(expr, _regex_static_bracket_start(expr)) == %d\n", count);

        shell_set_exit_code(-1);
        return -1;
    } 
    karray *aut = regex_new(argv[1]);
    for(int i = 0; i < aut->length; i++)
        regex_automaton_debug_print(((regex_automaton **)aut->array)[i]);
    bool m = regex_match(aut, argv[2]);
    kprintf("%s\n", m ? "true" : "false");
    karray_free(aut);
    shell_set_exit_code(m ? 0 : -1);
    return m ? 0 : -1;
}
int builtins_arrcmp(int argc, char **argv){
    karray *arr = _karray_static(((char *[]){"foo", "bar", "baz"}));
    uint16_t arr2[] = {0, 1, 0x1337};
    karray *arr3 = karray_new(sizeof(uint32_t), 0x0);
    for(int i = 0; i < 0xffff+1; i++){
        karray_push(arr3, i);
    }
    kprintf("arr.contains(\"foo1\") == %s\n",
        _karray_contains_fn(arr, (char *)"foo1", cmp_str) ? "true" : "false"
    );
    kprintf("arr.contains(\"bar\") == %s\n",
        _karray_contains_fn(arr, (char *)"bar", cmp_str) ? "true" : "false"
    );
    kprintf("arr2.contains(0x1337) == %s\n",
        _array_contains(arr2, 0x1337) ? "true" : "false"
    );
    kprintf("arr2.contains(2) == %s\n",
        _array_contains(arr2, 2) ? "true" : "false"
    );
    kprintf("arr2.contains(4) == %s\n",
        _array_contains(arr2, 4) ? "true" : "false"
    );
    kprintf("arr2.contains(1) == %s\n",
        _array_contains(arr2, 1) ? "true" : "false"
    );

    kprintf("arr3.contains(0xffff0) == %s\n",
        _karray_contains(arr3, 0xffff0) ? "true" : "false"
    );
    kprintf("arr3.contains(0xffff) == %s\n",
        _karray_contains(arr3, 0xffff) ? "true" : "false"
    );
    karray_free(arr3);

    for(char i = 'a'; i < 'h'; i++){
        kprintf("arr.contains('%c') == %s\n",
            (char)i,
            _karray_contains(
                _karray_static(((char []){'a', 'b', 'c'})),
                (char)i
            ) ? "true" : "false"
        );

    }
    return 0;
}

int builtins_net(int argc, char **argv){
    uint8_t icmp_request[] = {
        0x34, 0x49, 0x5b, 0xd9, 0x46, 0x9c, 0xa0, 0xce,
        0xc8, 0x94, 0x37, 0x33, 0x08, 0x00, 0x45, 0x00,
        0x00, 0x54, 0xc4, 0x15, 0x40, 0x00, 0x40, 0x01,
        0xb2, 0xdd, 0xc0, 0xa8, 0x01, 0x0c, 0x01, 0x01,
        0x01, 0x01, 0x08, 0x00, 0x89, 0x14, 0x00, 0x07,
        0x00, 0x01, 0xf9, 0xfc, 0xbf, 0x65, 0x00, 0x00,
        0x00, 0x00, 0xed, 0xad, 0x09, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
        0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
        0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25,
        0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
        0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
        0x36, 0x37    
    };
    uint8_t icmp_reply[] = {
        0xa0, 0xce, 0xc8, 0x94, 0x37, 0x33, 0x34, 0x49,
        0x5b, 0xd9, 0x46, 0x9c, 0x08, 0x00, 0x45, 0x00,
        0x00, 0x54, 0xc9, 0x66, 0x00, 0x00, 0x38, 0x01,
        0xf5, 0x8c, 0x01, 0x01, 0x01, 0x01, 0xc0, 0xa8,
        0x01, 0x0c, 0x00, 0x00, 0x91, 0x14, 0x00, 0x07,
        0x00, 0x01, 0xf9, 0xfc, 0xbf, 0x65, 0x00, 0x00,
        0x00, 0x00, 0xed, 0xad, 0x09, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
        0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
        0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25,
        0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
        0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
        0x36, 0x37

    };
    uint8_t udp[] = {
        0x01, 0x00, 0x5e, 0x00, 0x00, 0x07, 0xe0, 0x9d,
        0x13, 0xa7, 0x0a, 0x8c, 0x08, 0x00, 0x45, 0x00,
        0x00, 0xe2, 0x92, 0x93, 0x40, 0x00, 0x01, 0x11,
        0x44, 0x87, 0xc0, 0xa8, 0x01, 0x41, 0xe0, 0x00,
        0x00, 0x07, 0x1f, 0x41, 0x1f, 0x41, 0x00, 0xce,
        0xa1, 0xf5, 0x7b, 0x22, 0x64, 0x61, 0x74, 0x61,
        0x22, 0x3a, 0x7b, 0x22, 0x76, 0x31, 0x22, 0x3a,
        0x7b, 0x22, 0x75, 0x72, 0x69, 0x22, 0x3a, 0x22,
        0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x31,
        0x39, 0x32, 0x2e, 0x31, 0x36, 0x38, 0x2e, 0x31,
        0x2e, 0x36, 0x35, 0x3a, 0x38, 0x30, 0x30, 0x31,
        0x2f, 0x6d, 0x73, 0x2f, 0x31, 0x2e, 0x30, 0x2f,
        0x22, 0x7d, 0x2c, 0x22, 0x76, 0x32, 0x22, 0x3a,
        0x7b, 0x22, 0x75, 0x72, 0x69, 0x22, 0x3a, 0x22,
        0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x31,
        0x39, 0x32, 0x2e, 0x31, 0x36, 0x38, 0x2e, 0x31,
        0x2e, 0x36, 0x35, 0x3a, 0x38, 0x30, 0x30, 0x31,
        0x2f, 0x61, 0x70, 0x69, 0x2f, 0x76, 0x32, 0x2f,
        0x22, 0x7d, 0x7d, 0x2c, 0x22, 0x72, 0x65, 0x6d,
        0x6f, 0x74, 0x65, 0x22, 0x3a, 0x22, 0x31, 0x2e,
        0x30, 0x22, 0x2c, 0x22, 0x73, 0x69, 0x64, 0x22,
        0x3a, 0x22, 0x75, 0x75, 0x69, 0x64, 0x3a, 0x37,
        0x34, 0x35, 0x65, 0x30, 0x63, 0x31, 0x63, 0x2d,
        0x39, 0x66, 0x31, 0x37, 0x2d, 0x34, 0x37, 0x34,
        0x38, 0x2d, 0x39, 0x36, 0x38, 0x33, 0x2d, 0x66,
        0x35, 0x62, 0x33, 0x64, 0x65, 0x37, 0x39, 0x35,
        0x32, 0x34, 0x65, 0x22, 0x2c, 0x22, 0x74, 0x74,
        0x6c, 0x22, 0x3a, 0x38, 0x30, 0x30, 0x30, 0x2c,
        0x22, 0x74, 0x79, 0x70, 0x65, 0x22, 0x3a, 0x22,
        0x61, 0x6c, 0x69, 0x76, 0x65, 0x22, 0x7d, 0x0a
    };

    uint8_t http_req[] = {
        0x34, 0x49, 0x5b, 0xd9, 0x46, 0x9c, 0xa0, 0xce,
        0xc8, 0x94, 0x37, 0x33, 0x08, 0x00, 0x45, 0x00,
        0x00, 0x7e, 0x81, 0x2c, 0x40, 0x00, 0x40, 0x06,
        0xa0, 0x53, 0xc0, 0xa8, 0x01, 0x0c, 0xac, 0x40,
        0xab, 0x05, 0xb0, 0x1a, 0x00, 0x50, 0x49, 0xcd,
        0x75, 0x59, 0x0b, 0xfc, 0xb2, 0x9a, 0x80, 0x18,
        0x00, 0xfb, 0x84, 0x60, 0x00, 0x00, 0x01, 0x01,
        0x08, 0x0a, 0x0b, 0x1b, 0xf2, 0xed, 0x68, 0xda,
        0xe0, 0x3c, 0x47, 0x45, 0x54, 0x20, 0x2f, 0x20,
        0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e, 0x31,
        0x0d, 0x0a, 0x48, 0x6f, 0x73, 0x74, 0x3a, 0x20,
        0x69, 0x66, 0x63, 0x6f, 0x6e, 0x66, 0x69, 0x67,
        0x2e, 0x63, 0x6f, 0x0d, 0x0a, 0x55, 0x73, 0x65,
        0x72, 0x2d, 0x41, 0x67, 0x65, 0x6e, 0x74, 0x3a,
        0x20, 0x63, 0x75, 0x72, 0x6c, 0x2f, 0x38, 0x2e,
        0x32, 0x2e, 0x31, 0x0d, 0x0a, 0x41, 0x63, 0x63,
        0x65, 0x70, 0x74, 0x3a, 0x20, 0x2a, 0x2f, 0x2a,
        0x0d, 0x0a, 0x0d, 0x0a
    };

    uint8_t http_resp[] = {  
        0xa0, 0xce, 0xc8, 0x94, 0x37, 0x33, 0x34, 0x49,
        0x5b, 0xd9, 0x46, 0x9c, 0x08, 0x00, 0x45, 0x00,
        0x02, 0x7e, 0x4f, 0xae, 0x40, 0x00, 0x38, 0x06,
        0xd7, 0xd1, 0xac, 0x40, 0xab, 0x05, 0xc0, 0xa8,
        0x01, 0x0c, 0x00, 0x50, 0xb0, 0x1a, 0x0b, 0xfc,
        0xb2, 0x9a, 0x49, 0xcd, 0x75, 0xa3, 0x80, 0x18,
        0x00, 0x08, 0x4e, 0xea, 0x00, 0x00, 0x01, 0x01,
        0x08, 0x0a, 0x68, 0xda, 0xe0, 0x6b, 0x0b, 0x1b,
        0xf2, 0xed, 0x48, 0x54, 0x54, 0x50, 0x2f, 0x31,
        0x2e, 0x31, 0x20, 0x32, 0x30, 0x30, 0x20, 0x4f,
        0x4b, 0x0d, 0x0a, 0x44, 0x61, 0x74, 0x65, 0x3a,
        0x20, 0x53, 0x75, 0x6e, 0x2c, 0x20, 0x30, 0x34,
        0x20, 0x46, 0x65, 0x62, 0x20, 0x32, 0x30, 0x32,
        0x34, 0x20, 0x32, 0x31, 0x3a, 0x33, 0x34, 0x3a,
        0x35, 0x37, 0x20, 0x47, 0x4d, 0x54, 0x0d, 0x0a,
        0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d,
        0x54, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x74, 0x65,
        0x78, 0x74, 0x2f, 0x70, 0x6c, 0x61, 0x69, 0x6e,
        0x3b, 0x20, 0x63, 0x68, 0x61, 0x72, 0x73, 0x65,
        0x74, 0x3d, 0x75, 0x74, 0x66, 0x2d, 0x38, 0x0d,
        0x0a, 0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74,
        0x2d, 0x4c, 0x65, 0x6e, 0x67, 0x74, 0x68, 0x3a,
        0x20, 0x31, 0x36, 0x0d, 0x0a, 0x43, 0x6f, 0x6e,
        0x6e, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x3a,
        0x20, 0x6b, 0x65, 0x65, 0x70, 0x2d, 0x61, 0x6c,
        0x69, 0x76, 0x65, 0x0d, 0x0a, 0x43, 0x46, 0x2d,
        0x43, 0x61, 0x63, 0x68, 0x65, 0x2d, 0x53, 0x74,
        0x61, 0x74, 0x75, 0x73, 0x3a, 0x20, 0x44, 0x59,
        0x4e, 0x41, 0x4d, 0x49, 0x43, 0x0d, 0x0a, 0x52,
        0x65, 0x70, 0x6f, 0x72, 0x74, 0x2d, 0x54, 0x6f,
        0x3a, 0x20, 0x7b, 0x22, 0x65, 0x6e, 0x64, 0x70,
        0x6f, 0x69, 0x6e, 0x74, 0x73, 0x22, 0x3a, 0x5b,
        0x7b, 0x22, 0x75, 0x72, 0x6c, 0x22, 0x3a, 0x22,
        0x68, 0x74, 0x74, 0x70, 0x73, 0x3a, 0x5c, 0x2f,
        0x5c, 0x2f, 0x61, 0x2e, 0x6e, 0x65, 0x6c, 0x2e,
        0x63, 0x6c, 0x6f, 0x75, 0x64, 0x66, 0x6c, 0x61,
        0x72, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x5c, 0x2f,
        0x72, 0x65, 0x70, 0x6f, 0x72, 0x74, 0x5c, 0x2f,
        0x76, 0x33, 0x3f, 0x73, 0x3d, 0x31, 0x47, 0x32,
        0x6c, 0x30, 0x76, 0x30, 0x41, 0x45, 0x48, 0x42,
        0x65, 0x25, 0x32, 0x42, 0x69, 0x42, 0x49, 0x25,
        0x32, 0x46, 0x59, 0x4f, 0x74, 0x38, 0x31, 0x33,
        0x62, 0x47, 0x39, 0x55, 0x75, 0x58, 0x6a, 0x4d,
        0x65, 0x32, 0x73, 0x38, 0x52, 0x6a, 0x45, 0x35,
        0x38, 0x49, 0x58, 0x52, 0x57, 0x51, 0x70, 0x6d,
        0x35, 0x79, 0x58, 0x39, 0x54, 0x6b, 0x6d, 0x6b,
        0x32, 0x34, 0x6c, 0x79, 0x4d, 0x67, 0x4c, 0x37,
        0x79, 0x54, 0x66, 0x36, 0x6e, 0x53, 0x48, 0x6c,
        0x58, 0x38, 0x4f, 0x64, 0x71, 0x76, 0x70, 0x41,
        0x76, 0x51, 0x57, 0x69, 0x39, 0x4e, 0x36, 0x73,
        0x57, 0x33, 0x37, 0x33, 0x77, 0x33, 0x46, 0x25,
        0x32, 0x46, 0x4f, 0x76, 0x47, 0x77, 0x54, 0x32,
        0x59, 0x41, 0x6c, 0x38, 0x44, 0x4b, 0x67, 0x42,
        0x6b, 0x73, 0x4b, 0x6b, 0x39, 0x6d, 0x70, 0x44,
        0x4e, 0x44, 0x33, 0x35, 0x36, 0x38, 0x58, 0x4e,
        0x77, 0x25, 0x33, 0x44, 0x25, 0x33, 0x44, 0x22,
        0x7d, 0x5d, 0x2c, 0x22, 0x67, 0x72, 0x6f, 0x75,
        0x70, 0x22, 0x3a, 0x22, 0x63, 0x66, 0x2d, 0x6e,
        0x65, 0x6c, 0x22, 0x2c, 0x22, 0x6d, 0x61, 0x78,
        0x5f, 0x61, 0x67, 0x65, 0x22, 0x3a, 0x36, 0x30,
        0x34, 0x38, 0x30, 0x30, 0x7d, 0x0d, 0x0a, 0x4e,
        0x45, 0x4c, 0x3a, 0x20, 0x7b, 0x22, 0x73, 0x75,
        0x63, 0x63, 0x65, 0x73, 0x73, 0x5f, 0x66, 0x72,
        0x61, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x22, 0x3a,
        0x30, 0x2c, 0x22, 0x72, 0x65, 0x70, 0x6f, 0x72,
        0x74, 0x5f, 0x74, 0x6f, 0x22, 0x3a, 0x22, 0x63,
        0x66, 0x2d, 0x6e, 0x65, 0x6c, 0x22, 0x2c, 0x22,
        0x6d, 0x61, 0x78, 0x5f, 0x61, 0x67, 0x65, 0x22,
        0x3a, 0x36, 0x30, 0x34, 0x38, 0x30, 0x30, 0x7d,
        0x0d, 0x0a, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72,
        0x3a, 0x20, 0x63, 0x6c, 0x6f, 0x75, 0x64, 0x66,
        0x6c, 0x61, 0x72, 0x65, 0x0d, 0x0a, 0x43, 0x46,
        0x2d, 0x52, 0x41, 0x59, 0x3a, 0x20, 0x38, 0x35,
        0x30, 0x36, 0x30, 0x61, 0x36, 0x39, 0x62, 0x38,
        0x64, 0x38, 0x37, 0x38, 0x34, 0x31, 0x2d, 0x43,
        0x44, 0x47, 0x0d, 0x0a, 0x61, 0x6c, 0x74, 0x2d,
        0x73, 0x76, 0x63, 0x3a, 0x20, 0x68, 0x33, 0x3d,
        0x22, 0x3a, 0x34, 0x34, 0x33, 0x22, 0x3b, 0x20,
        0x6d, 0x61, 0x3d, 0x38, 0x36, 0x34, 0x30, 0x30,
        0x0d, 0x0a, 0x0d, 0x0a, 0x31, 0x37, 0x36, 0x2e,
        0x31, 0x34, 0x31, 0x2e, 0x31, 0x34, 0x32, 0x2e,
        0x32, 0x31, 0x37, 0x0a
    };
    net_debug_print_packet(icmp_request, sizeof(icmp_request)/sizeof(icmp_request[0]));
    net_debug_print_packet(icmp_reply, sizeof(icmp_reply)/sizeof(icmp_reply[0]));
    net_debug_print_packet(udp, sizeof(udp)/sizeof(udp[0]));
    net_debug_print_packet(http_req, sizeof(http_req)/sizeof(http_req[0]));
    net_debug_print_packet(http_resp, sizeof(http_resp)/sizeof(http_resp[0]));

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
const int builtins_testvm(int argc, char **argv){
    char *in[] = {
        "foobar = 100;",
        "F00bar = \"12\";",
        "\tF00bar = \"12\";",
        "F00bar    = \"12\";",
        "char a = b;",
        "char * a = b;",
        "a=b;",
        "char* f = \"foobar\";",
        "char * f = \"foobar\";",
        "int x = 0;",
        "char* a=b;",
        "char *   f =   \"hello\" ;",
        "void **foobar(){}",
        "int * strlen () {}",

    };
    for(int i = 0; i < sizeof(in)/sizeof(in[0]); i++){
        parse_code((uint8_t *)in[i]);
    }
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
#define _BUILTIN(s,f) {(uint64_t)s, (uint64_t)f}
uint64_t _shell_builtins[][2] = {
    _BUILTIN("help", builtins_help),
    _BUILTIN("graphics", builtins_graphics),
    _BUILTIN("gol", builtins_gol),
    _BUILTIN("arrcmp", builtins_arrcmp),
    _BUILTIN("testscroll", builtins_testscroll),
    _BUILTIN("regex", builtins_regex),
    _BUILTIN("poweroff", builtins_poweroff),
    _BUILTIN("help", builtins_help),
    _BUILTIN("testvm", builtins_testvm)
};
int builtins_help(int argc, char **argv){
    for(int i = 0 ; i < _builtins_size(); i++)
        kprintf("%s\n", _shell_builtins[i][_BUILTIN_NAME]);
    return 0;
}
inline int _builtins_size(){
    return _BUILTINS_SIZE;
}
