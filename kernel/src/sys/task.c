#include <sys/task.h>

task *task_current = 0x0;
task *task_first = 0x0;
task *task_last = 0x0;
uint64_t task_last_tick = 0x0;
uint64_t task_lock_counter = 0;
uint64_t task_time_total_available = TASK_CPU_TIME_TOTAL;

extern uint64_t kalloc_list_alloc;
extern uint64_t kalloc_list_last;

void task_end(task *t){
    if(!t)
        return;
    t->status = task_status_ended;
}
void task_end_current(){
    task_end(task_current);
}
void task_end_wait(task *t){
    while(task_status_get(t) != task_status_ended){
        sleep(1);
    }
}
task_status task_status_get(task *t){
    if(!t){
        return task_status_ended;
    }
    task_lock();
    task *t2 = task_first;
    while(t2){
        if(t == t2){
            task_unlock();
            return t2->status;
        }
        t2 = t2->next;
    }
    task_unlock();
    return task_status_ended;
}
void task_lock(){
    interrupt_disable();
    task_lock_counter++;
}
void task_unlock(){
    task_lock_counter--;
    if(!task_lock_counter)
        interrupt_enable();
}
void task_allocation_remove(task *t){
    if(!t){
        KERROR("t == NULL");
        return;
    }
    task_lock();
    for(uint64_t i = 0; i < kalloc_list_alloc; i++){
        if(kalloc_list[i].task == t){
            kheap_free_mem2(&heap, &kalloc_list[i]);
            memset(kalloc_list+i, 0, sizeof(kheap_allocated_block));
        }
    }
    task_unlock();
}
void task_allocation_add(kheap_allocated_block *b){
    b->task = task_current;
}
task *task_new(int(*fn)(void *, task *), void *data, char *name, task_priority priority){
    if(!fn){
        KERROR("fn == 0x0");
        return 0x0;
    }
    task_lock();
    task *ret = kmalloc(sizeof(task));
    *ret = (task){
        .next = 0x0,
        .prev = task_last,  // task_last == 0x0 ? task_last : task_last;
        .context = kcalloc(sizeof(cpu_registers),1),
        .stack_end = kcalloc(TASK_STACK_SIZE, 1),
        .status = task_status_created,
        .fn = fn,
        .data = data,
        .name = name == 0x0 ? strdup("(null)") : strdup(name),
        .priority = priority
    };
    switch (ret->priority){
        case task_priority_high:
            ret->time_slice = TASK_CPU_TIME_HIGH;
            break;
        case task_priority_medium:
            ret->time_slice = TASK_CPU_TIME_MEDIUM;
            break;
        case task_priority_low:
            ret->time_slice = TASK_CPU_TIME_LOW;
            break;
        case task_priority_sleep:
            ret->time_slice = TASK_CPU_TIME_SLEEP;
            break;
        default:
            KMESSAGE("task_current->priority == %d (unknown) defaulting to TASK_CPU_TIME_MEDIUM",
                (uint64_t)task_current->priority
            );
            ret->time_slice = TASK_CPU_TIME_MEDIUM;
            break;
    }
    ret->stack_start = (void *)((uint64_t)ret->stack_end + (TASK_STACK_SIZE));
    ret->context->CPU_REGISTER_STACK = (uint64_t)ret->stack_start;
    if(!task_first){
        task_first = ret;
    }
    if(task_last){
        task_last->next = ret;
    }
    task_last = ret;
    task_unlock();
    return ret;
}
task_priority task_priority_get(task *t){
    return t == 0x0 ? task_priority_medium : t->priority;
}
void task_priority_set(task *t, task_priority p){
    if(!t){
        KERROR("t == NULL");
        return;
    }
    switch(p){
        case task_priority_high:
            t->time_slice = TASK_CPU_TIME_HIGH;
            break;
        case task_priority_medium:
            t->time_slice = TASK_CPU_TIME_MEDIUM;
            break;
        case task_priority_low:
            t->time_slice = TASK_CPU_TIME_LOW;
            break;
        case task_priority_sleep:
            t->time_slice = TASK_CPU_TIME_SLEEP;
            break;
        default:
            KERROR("p == %d (unknown)", p);
            break;
    }
}
void task_free(task *t){
    if(!t)
        return;
    /*
    KMESSAGE(
        "freeing task %s (0x%x)",
        t->name,
        t
    );
    */
    kfree(t->stack_end);
    kfree(t->context);
    kfree(t->name);
    task_allocation_remove(t);
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
    task_last = 0x0;
    task_first = 0x0;
    task_current = 0x0;
    kfree(t);
    KMESSAGE("No more tasks need to reload shell");
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
            t
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
    if(!task_first){
        return;
    }
    if(!task_current){
        task_current = task_first;
    }
    task_current->time_slice_remaining -= cpu_get_tick() - task_last_tick;

    if(task_current->time_slice_remaining <= 0){
        /*
        KMESSAGE("switching task %s time_slice == %d", 
            task_current->name,
            (uint64_t)(task_time_total_available >> task_current->time_slice)
        );
        
        kgetchar();
        */
        task_current->time_slice_remaining = task_time_total_available >> task_current->time_slice;
        task_current = task_get_next();
    }

    task_last_tick = cpu_get_tick();
    switch (task_current->status){
        case task_status_created:
            //run the task
            task_current->status = task_status_running;
            task_current->time_slice_remaining = task_time_total_available >> task_current->time_slice;
            task_cpu_registers_load(
                task_current->context,
                (void *)task_run,
                task_current->data,
                task_current
            );
            break;
        case task_status_ended:
            task_free(task_current);
            task_current = task_get_next();
            task_scheduler();
            break;
        case task_status_running:
            task_cpu_registers_reload(task_current->context);
            break;
        case task_status_wait_io:
            task_scheduler();
            break;
        default:
            KPANIC("task_current (0x%x) :\n\ttask_current->status == %d (unknown)",
                task_current,
                task_current->status
            );
            break;
    }
    task_scheduler();
}
