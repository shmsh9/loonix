#include <process.h>

karray *process_list = 0x0;
uint32_t process_last_id = 0;
uint32_t process_current = 0;

process * process_new(int (*fn)(int, char **), int argc, char **argv){
    process *ret = kmalloc(sizeof(process));
    *ret = (process){
        .id = process_last_id++,
        .priority = 1,
        .stack = kmalloc(PROCESS_STACK_SIZE),
        .registers = kcalloc(sizeof(cpu_registers), 1),
        .argc = argc,
        .argv = argv
    };
    ret->registers->rsp = (uintptr_t)ret->stack - PROCESS_STACK_SIZE;
    ret->registers->rip = (uint64_t)fn;
    karray_push(process_list, (uint64_t)ret);
    return ret;
}

void process_karray_free(process *proc){
    kfree(proc->stack);
    kfree(proc->registers);
    kfree(proc);
}

void process_init(){
    process_list = karray_new(
        sizeof(process *),
        (void (*)(void *))process_karray_free
    );
}

void process_free(uint32_t pid){
    for(uint32_t i = 0; i < process_list->length; i++){
        process *tmp = ((process **)(process_list->array))[i];
        if(tmp->id == pid){
            karray_pop(process_list, pid);
            return;
        }
    }
    KMESSAGE("%d not found", pid);
}

void process_scheduler(){
    KMESSAGE("start");
    if(!process_list)
        return;
    if(process_list->length == 0)
        return;
    //process *proc = ((process **)(process_list->array))[process_current];
    if(process_current+1 >= process_list->length){
        process_current = 0;
    }
    else{
        process_current++;
    }
    KMESSAGE("loading reg for proc[%d]", (uint64_t)process_current);
    //cpu_registers_load(proc->registers);
}
