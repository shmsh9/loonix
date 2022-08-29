#include <arch/aarch64.h>
#include <kstd.h>
#ifdef __aarch64__
extern void *interrupt_vector_table;
extern uint64_t interrupt_functions_table[256];
//https://krinkinmu.github.io/2021/01/10/aarch64-interrupt-handling.html
void interrupt_handler(uint64_t far_el1, uint64_t esr_el1){
    KPANIC("\nunhandled exception:\n\t[far_el1] : 0x%x\n\t[esr_el1] : 0b%b",
        far_el1,
        esr_el1
    );
}
#endif
