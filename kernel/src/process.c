#include <process.h>

karray *process_list = 0x0;
uint32_t process_last_id = 0;

process * process_new(int (*fn)(int, char **), int argc, char **argv){
    process *ret = kmalloc(sizeof(process));
    *ret = (process){
        .id = process_last_id++,
        .priority = 1,
        .stack = kmalloc(PROCESS_STACK_SIZE),
        .execution_pointer = (void *)fn,
        .argc = argc,
        .argv = argv
    };
    karray_push(process_list, (uint64_t)ret);
    return ret;
}

void process_karray_free(process *proc){
    kfree(proc->stack);
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

