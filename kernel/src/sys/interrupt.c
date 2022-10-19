#include <arch/arch.h>
#include <kstd/kstd.h>
#include <drivers/rtc.h>

void *current_interrupt_frame[4] = {0x0};

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
void interrupt_handler_null(){
    return;
}
void interrupt_handler_zerodiv(){
    KPANICINTERRUPT("division by zero attempted", current_interrupt_frame);
}
void interrupt_handler_invalid_opcode(){
    KPANICINTERRUPT("Invalid OP Code", current_interrupt_frame);
}
void interrupt_handler_breakpoint(){
    KPANICINTERRUPT("Breakpoint", current_interrupt_frame);
}
void interrupt_handler_timer(){
    KMESSAGE("Timer fired");
}
void interrupt_general_protection_fault(){
    KPANICINTERRUPT("protection fault !", current_interrupt_frame);
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

