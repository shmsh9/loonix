#include <shell/builtins.h>
#include <mem.h>
KHEAPBM HEAP;
void __init_glob(){
    k_heapBMInit(&HEAP);                              /* initialize the heap */
    k_heapBMAddBlock(&HEAP, __HEAP_START, 0x100, 16);  /* add block to heap */
    uint32_t c = 0;
    SHELL_INIT_BUILTIN(c, clear, "clear");
    SHELL_INIT_BUILTIN(c, help, "help");
}
