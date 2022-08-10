#include <shell/builtins.h>
#include <newmem.h>
#include <kernel.h>
 
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

kheap_allocated_block kalloc_list[KALLOC_LIST_MAX] = {0};
kheap heap;
uint32_t kalloc_list_last = 0;

void __init_glob(){
    builtins.length = 0;
    SHELL_INIT_BUILTIN(clear, "clear");
    SHELL_INIT_BUILTIN(help, "help");
    SHELL_INIT_BUILTIN(useless, "1234567812345678");
}
