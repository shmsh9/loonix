#include <arch/arch.h>
#include <kstd/kstd.h>
#include <drivers/rtc.h>
#include <sys/task.h>

void *current_interrupt_frame[4] = {0x0};

#define KERRORINTERRUPT(msg, interrupt_frame){\
    kprintf("[%d][kernel][interrupt] : %s\n", (uint64_t)cpu_get_tick() ,msg);\
    if(!task_current){\
        STACKTRACE();\
    }\
    else{\
        kprintf("task 0x%x (%s) : \n", (uint64_t)task_current, task_current->name);\
        STACKTRACE_CTXT(task_current->context->CPU_REGISTER_FRAME);\
        CPU_REGISTERS_PRINT(task_current->context);\
    }\
    INTERRUPT_FRAME_PRINT(interrupt_frame);\
    framebuffer_device_update(fb);\
}
#define KPANICINTERRUPT(msg, interrupt_frame) {\
    task_lock();\
    KERRORINTERRUPT(msg, interrupt_frame);\
    task_end_current();\
    task_unlock();\
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
    task_lock();
    KERRORINTERRUPT("Debug", current_interrupt_frame);
    task_unlock();
}
void interrupt_handler_timer(){
    KMESSAGE("Timer fired");
}
void interrupt_general_protection_fault(){
    task_lock();
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
        KERROR("interrupt %d is already installed", (uint64_t)num);
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

