#include <irq/aarch64.h>
#include <kstd.h>
void common_trap_handler(){
    kprint("TRAPPED !\n");
}

