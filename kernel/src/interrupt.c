#include <arch/arch.h>
#include <kstd.h>
#include <drivers/rtc.h>

#define KERRORINTERRUPT(msg, interrupt_frame){\
    kprintf("[%d][kernel][interrupt] : %s\n", cpu_get_tick() ,msg);\
    STACKTRACE();\
    INTERRUPT_FRAME_PRINT(interrupt_frame);\
    framebuffer_device_update(fb);\
}
#define KPANICINTERRUPT(msg, interrupt_frame) {\
    KERRORINTERRUPT(msg, interrupt_frame);\
    BREAKPOINT();\
}
uint64_t interrupt_functions_table[INTERRUPT_FUNCTIONS_TABLE_SIZE] = {0};
void interrupt_handler_null(void *interrupt_frame){
    return;
}
void interrupt_handler_zerodiv(void *interrupt_frame){
    KERRORINTERRUPT("division by zero attempted", interrupt_frame);
}
void interrupt_handler_invalid_opcode(void *interrupt_frame){
    KPANICINTERRUPT("Invalid OP Code", interrupt_frame);
}
void interrupt_handler_breakpoint(void *interrupt_frame){
    KPANICINTERRUPT("Breakpoint", interrupt_frame);
}
void interrupt_handler_timer(void *interrupt_frame){
    KMESSAGE("Timer fired");
}
void interrupt_general_protection_fault(void *interrupt_frame){
    KPANICINTERRUPT("protection fault !", interrupt_frame);
}
void interrupt_functions_table_init(){
    memset(interrupt_functions_table, 0, INTERRUPT_FUNCTIONS_TABLE_SIZE*sizeof(uint64_t));
    INTERRUPT_FUNCTIONS_INSTALL_DEFAULT_ARCH();
}
void interrupt_handler_install(void (*fn)(), uint16_t num){
    if(num >= INTERRUPT_FUNCTIONS_TABLE_SIZE){
        KERROR("interrupt num exceeds %d", INTERRUPT_FUNCTIONS_TABLE_SIZE);
        return;
    }
    if(interrupt_functions_table[num]){
        KERROR("interrupt %d is already installed");
        return;
    }
    interrupt_functions_table[num] = (uint64_t)fn;
}

void interrupt_handler_replace(void (*fn)(), uint16_t num){
    if(num >= INTERRUPT_FUNCTIONS_TABLE_SIZE){
        KERROR("interrupt num exceeds %d", INTERRUPT_FUNCTIONS_TABLE_SIZE);
        return;
    }
    interrupt_functions_table[num] = (uint64_t)fn;
}

