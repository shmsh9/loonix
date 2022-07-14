#include <shell/builtins.h>
#include <mem.h>
void __init_glob(){
    k_heapBMInit(&__HEAP_START);                              /* initialize the heap */
    k_heapBMAddBlock(&__HEAP_START, 0x100000, 0x100000, 16);  /* add block to heap */
    uint32_t c = 0;
    SHELL_INIT_BUILTIN(c, clear, "clear");
    SHELL_INIT_BUILTIN(c, help, "help");
}
