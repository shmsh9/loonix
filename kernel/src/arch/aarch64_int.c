#include <arch/aarch64.h>
#include <kstd/kstd.h>
#ifdef __aarch64__
extern void *interrupt_vector_table;
extern uint64_t interrupt_functions_table[256];
//https://krinkinmu.github.io/2021/01/10/aarch64-interrupt-handling.html
#define INTERRUPT_TYPE_NONE  0
#define INTERRUPT_TYPE_IRQ   1
#define INTERRUPT_TYPE_FIQ   2
#define INTERRUPT_TYPE_ERROR 3

uint64_t esr_get_exception_class(uint64_t esr_el1){
    return (esr_el1 >> 26 ) & 0x3f; //(0b111111 6bits)
}
uint64_t esr_get_iss(uint64_t esr_el1){
    return esr_el1 & 0xffffff;
}
uint64_t esr_get_iss2(uint64_t esr_el1){
    return (esr_el1 >> 32 ) & 0x1f;
}
void svc_interrupt_handler(uint64_t esr_el1){
    uint64_t iss = esr_get_iss(esr_el1);
    if(interrupt_functions_table[iss]){
        ((void (*)())interrupt_functions_table[iss])();
        return;
    }
    KPANIC("unhandled svc #%d", iss);
}
//https://developer.arm.com/documentation/ddi0601/2021-12/AArch64-Registers/ESR-EL1--Exception-Syndrome-Register--EL1-
void interrupt_handler(uint64_t far_el1, uint64_t esr_el1, uint64_t interrupt_type){
    switch(esr_get_exception_class(esr_el1)){
        case 0x15 ://0b010101
            svc_interrupt_handler(esr_el1);
            return;
            break;
        default:
            break;
    }
    KPANIC("\nunhandled exception:\n\t[far_el1] : 0x%x\n\t[esr_el1] : 0b%b\n\t[class  ] : 0b0%b\n\t[iss    ] : 0b%b\n\t[type   ] : %d",
        far_el1,
        esr_el1,
        esr_get_exception_class(esr_el1),
        esr_get_iss(esr_el1),
        interrupt_type
    );
}
#endif
