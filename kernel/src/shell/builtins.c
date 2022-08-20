#include <shell/builtins.h>

struct fnbuiltins builtins = {0};

int builtins_clear(int argc, char **argv){
    kprint("\033[2J\033[H");
    return 0;
}
int builtins_help(int argc, char **argv){
    for(int i = 0 ; i < builtins.length; i++){
        kprint(builtins.builtins[i].name);
        kputc('\n');
    }
    return 0;
}

int builtins_testkarray(int argc, char **argv){
    karray *k = karray_new(sizeof(uint64_t),NULL);
    for(uint64_t i = 0; i < 0xfffff; i++)
        karray_push(k, i);
    //karray_debug_print(k);
    karray_free(k);
    return 0;
}
int builtins_testklist(int argc, char **argv){
    klist *k = klist_new(NULL);
    for(uint64_t i = 0; i < 0xffff; i++){
        klist_push(k,i);
    }
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
    #define TEST_STRDUP_SZ 0xff
    char **arr = kcalloc(sizeof(char *),TEST_STRDUP_SZ);
    for(int i = 0; i < TEST_STRDUP_SZ; i++){
        arr[i] = strdup("12345");
    }
    for(int i = 0; i < TEST_STRDUP_SZ; i++){
        kfree(arr[i]);
    }
    kfree(arr);
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

void builtins_init(){
    builtins.length = 0;
    BUILTINS_INIT_FN(builtins_help, "help");
    BUILTINS_INIT_FN(builtins_clear, "clear");
    BUILTINS_INIT_FN(builtins_free, "free");
    BUILTINS_INIT_FN(builtins_testkarray, "testkarray");
    BUILTINS_INIT_FN(builtins_testklist, "testklist");
    BUILTINS_INIT_FN(builtins_testkcalloc, "testkcalloc");
    BUILTINS_INIT_FN(builtins_teststrdup, "teststrdup");
    BUILTINS_INIT_FN(builtins_testscroll, "testscroll");

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
