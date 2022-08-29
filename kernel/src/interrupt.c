#include <arch/arch.h>
#include <kstd.h>
uint64_t interrupt_functions_table[INTERRUPT_FUNCTIONS_TABLE_SIZE] = {0};

void interrupt_handler_zerodiv(){
    KERROR("division by zero attempted");
}
void interrupt_handler_int3(){
    KPANIC("INT3!");
}
void interrupt_functions_table_init(){
    memset(interrupt_functions_table, 0, INTERRUPT_FUNCTIONS_TABLE_SIZE*sizeof(uint64_t));

    interrupt_handler_install(interrupt_handler_zerodiv, 0);
    interrupt_handler_install(interrupt_handler_int3, 3);
}
void interrupt_handler_install(void (*fn)(), uint16_t num){
    if(num >= INTERRUPT_FUNCTIONS_TABLE_SIZE){
        KERROR("interrupt num exceeds %d", INTERRUPT_FUNCTIONS_TABLE_SIZE);
        return;
    }
    interrupt_functions_table[num] = (uint64_t)fn;
}


