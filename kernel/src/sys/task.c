#include <sys/task.h>

task *task_current = 0x0;
task *task_first = 0x0;
task *task_last = 0x0;

void task_end(task *t){
    t->status = task_status_ended;
}

task *task_new(void(*fn)(void *, task *), void *data){
    task *ret = kmalloc(sizeof(task));
    *ret = (task){
        .next = 0x0,
        .prev = task_last,  // task_last == 0x0 ? task_last : task_last;
        .uuid = cpu_get_tick(),
        .context = kcalloc(sizeof(cpu_registers),1),
        .stack = kmalloc(TASK_STACK_SIZE),
        .status = task_status_created,
        .priority = 0,
        .fn = fn,
        .data = data
    };
    if(!task_first){
        task_first = ret;
    }
    if(task_last){
        task_last->next = ret;
    }
    task_last = ret;
    return ret;
}
void task_free(task *t){
    kfree(t->stack);
    kfree(t->context);
    if(t->next && t->prev){
        t->next->prev = t->prev;
        t->prev->next = t->next;
        kfree(t);
        return;
    }
    //t was the first task
    if(t->next && !t->prev){
        t->next->prev = 0x0;
        task_first = t->next;
        task_last = t->next;
        kfree(t);
        return;
    }
    //t was the last task
    if(t->prev && !t->next){
        t->prev->next = 0x0;
        task_last = t->prev;
        kfree(t);
        return;
    }
    KMESSAGE("t->next && t->prev == 0x0");
    task_last = 0x0;
    task_first = 0x0;
    task_current = 0x0;
    kfree(t);
}

void task_debug_print(){
    if(!task_first){
        KMESSAGE("task_first == null");
        return;
    }
    task *t = task_first;
    uint64_t i = 0;
    kprintf("Tasks :\n");
    while(t){
        kprintf("\tTask %d : 0x%x\n", 
            i,
            t->uuid
        );
        CPU_REGISTERS_PRINT(t->context);
        i++;
        t = t->next;
    }
}
task *task_get_next(){
    return task_current->next == 0 ? task_first : task_current->next;
}
void task_scheduler(){
    rtc_device_time_since_boot_centisecond++; //uglross
    if(!task_first)
        return;
    if(!task_current){
        task_current = task_first;
    }
    else{
        task_current = task_get_next(); 
    }
    switch (task_current->status){
    case task_status_created:
        //run the task
        //fakerun
        task_current->status = task_status_running;
        task_interrupt_enable();
        task_current->fn(task_current->data, task_current);
        break;
    case task_status_ended:
        task_free(task_current);
        task_current = task_get_next();
        break;
    case task_status_running:
        task_interrupt_enable();
        //reload task context
        //cpu_registers_load(task_current->context);
        break;
    default:
        KPANIC("task_current (0x%x) :\n\ttask_current->status == %d (unknown)",
            task_current,
            task_current->status
        );
        break;
    }
    // task_current->fn != 0x0 not the kmain task
    /*
    if(task_current->context->rip != 0){
        KMESSAGE("\nCaptured task_current :\n\n");
        STACKTRACE_CTXT(task_current->context->rbp);
        CPU_REGISTERS_PRINT(task_current->context);
        BREAKPOINT();
    }
    */
}
