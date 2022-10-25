#include <sys/task.h>

task *task_current = 0x0;
task *task_first = 0x0;
task *task_last = 0x0;

void task_end(task *t){
    if(!t)
        return;
    t->status = task_status_ended;
}
void task_lock(){
    interrupt_disable();
}
void task_unlock(){
    interrupt_enable();
}
void *task_kmalloc(uint32_t sz){
    task_lock();
    void *ret = kmalloc(sz);
    task_unlock();
    return ret;
}
task *task_new(int(*fn)(void *, task *), void *data, char *name){
    if(!fn){
        KERROR("fn == 0x0");
        return 0x0;
    }
    task *ret = kmalloc(sizeof(task));
    *ret = (task){
        .next = 0x0,
        .prev = task_last,  // task_last == 0x0 ? task_last : task_last;
        .uuid = cpu_get_tick(),
        .context = kcalloc(sizeof(cpu_registers),1),
        .stack_end = kcalloc(TASK_STACK_SIZE, 1),
        .status = task_status_created,
        .priority = 0,
        .fn = fn,
        .data = data,
        .name = name == 0x0 ? "(null)" : strdup(name)
    };
    ret->stack_start = (void *)((uint64_t)ret->stack_end + (TASK_STACK_SIZE));
    ret->context->CPU_REGISTER_STACK = (uint64_t)ret->stack_start;
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
    if(!t)
        return;
    kfree(t->stack_end);
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
        kfree(t);
        return;
    }
    //t was the last task
    if(t->prev && !t->next && t == task_last){
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
    kprintf("number\tname\taddress\n");
    while(t){
        kprintf("%d\t%s\t0x%x\n",
            i, 
            t->name,
            t->uuid
        );
        //CPU_REGISTERS_PRINT(t->context);
        i++;
        t = t->next;
    }
}
task *task_get_next(){
    if(!task_current)
        return 0x0;
    return task_current->next == 0x0 ? task_first : task_current->next;
}
void task_run(void *data, task *t){
    t->fn(data, t);
    task_end(t);
    while(1){}
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
    /*
    KMESSAGE("task_current == 0x%x task_current->stack == 0x%x", 
        task_current,
        task_current->stack_start
    );
    */
    switch (task_current->status){
    case task_status_created:
        //run the task
        task_current->status = task_status_running;
        //need abstraction for other ARCH
        /*
        task_current->context->rsp = (uint64_t)task_current->stack_start;
        task_current->context->rip = (uint64_t)task_run;
        task_current->context->rdi = (uint64_t)task_current->data;
        task_current->context->rsi = (uint64_t)task_current;
        */
        //void task_cpu_registers_load(cpu_registers *rdi, void *rsi (fn), void *rdx (data), task *rcx (t));
        task_cpu_registers_load(
            task_current->context,
            (void *)task_run,
            task_current->data,
            task_current
        );
        break;
    case task_status_ended:
        task_free(task_current);
        //task_current = task_get_next();
        task_scheduler();
        break;
    case task_status_running:
        task_cpu_registers_reload(task_current->context);
        break;
    case task_status_waiting:
        /*
        if(task_current->task_wait_over){
            task_current->status = task_status_running;
        }
        */
        task_scheduler();
        break;
    default:
        KPANIC("task_current (0x%x) :\n\ttask_current->status == %d (unknown)",
            task_current,
            task_current->status
        );
        break;
    }
}
