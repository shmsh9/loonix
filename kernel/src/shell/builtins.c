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
    uint64_t mem = MEMORY_LEAK_TEST_START();
    karray *k = karray_new(sizeof(uint64_t),kfree);
    for(uint64_t i = 0; i < 0xff; i++)
        karray_push(k, (uint64_t)strdup("foobar"));
    karray_debug_print(k);
    karray_free(k);
    MEMORY_LEAK_TEST_STOP(mem);
    return 0;
}
int builtins_testklist(int argc, char **argv){
    uint64_t mem = MEMORY_LEAK_TEST_START();
    klist *k = klist_new(kfree);
    for(uint64_t i = 0; i < 10; i++){
        klist_push(k, (uint64_t)strdup("123567"));
    }
    klist_free(k);
    MEMORY_LEAK_TEST_STOP(mem);
    return 0;
}
int builtins_testkcalloc(int argc, char **argv){
    uint64_t mem = MEMORY_LEAK_TEST_START();
    #define TEST_KCALLOC_SZ 512
    void *arr[TEST_KCALLOC_SZ] = {0};
    for(int i = 0; i < TEST_KCALLOC_SZ; i++){
        arr[i] = kcalloc(8,1);
    }
    for(int i = 0; i < TEST_KCALLOC_SZ; i++){
        kfree(arr[i]);
    }
    MEMORY_LEAK_TEST_STOP(mem);
    return 0;
}

int builtins_teststrdup(int argc, char **argv){
    uint64_t mem = MEMORY_LEAK_TEST_START();
    #define TEST_STRDUP_SZ 0xff
    char **arr = kmalloc(sizeof(char *)*TEST_STRDUP_SZ);
    for(int i = 0; i < TEST_STRDUP_SZ; i++){
        arr[i] = strdup("12345");
    }
    for(int i = 0; i < TEST_STRDUP_SZ; i++){
        kfree(arr[i]);
    }
    kfree(arr);
    MEMORY_LEAK_TEST_STOP(mem);
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
