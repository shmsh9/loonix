#include <arch/aarch64.h>
#include <kstd.h>
#ifdef __aarch64__
extern void *interrupt_vector_table;
//https://krinkinmu.github.io/2021/01/10/aarch64-interrupt-handling.html
void interrupt_handler(){
    KPANIC("unhandled exception");
}
#endif
