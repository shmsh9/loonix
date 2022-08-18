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

void builtins_init(){
    builtins.length = 0;
    BUILTINS_INIT_FN(builtins_help, "help");
    BUILTINS_INIT_FN(builtins_clear, "clear");
    BUILTINS_INIT_FN(builtins_free, "free");

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
