#include <sys/task.h>

task *task_current = 0x0;
task *task_first = 0x0;
task *task_last = 0x0;
uint64_t task_last_tick = 0x0;
uint64_t task_lock_counter = 0;
uint64_t task_time_total_available = TASK_CPU_TIME_TOTAL;
bool task_new_memory_allocation = false;

extern uint64_t kalloc_list_alloc;
extern uint64_t kalloc_list_last;

void task_end(task *t){
    switch((uintptr_t)t){
        case 0x0:
            break;
        default:
            task_allocation_remove(t);
            t->status = task_status_ended;
            break;
    }
}
void task_end_later(task *t){
    if(!t){
        KERROR("t == NULL");
        return;
    }
    t->status = task_status_ended;
}
void task_end_current(){
    task_end_later(task_current);
}
void task_end_wait(task *t){
    task_current->waiting_on = t;
    task_pause(task_current);
}
task_status task_status_get(task *t){
    return t ? t->status : task_status_ended;
}
void task_lock(){
    interrupt_disable();
    task_lock_counter++;
}
void task_unlock(){
    task_lock_counter--;
    switch(task_lock_counter){
        case 0:
            interrupt_enable();
            break;
        default:
            break;
    }
}
void task_allocation_remove(task *t){
    if(!t){
        KERROR("t == NULL");
        return;
    }
    task_lock();
    for(uint64_t i = 0; i < kalloc_list_alloc; i++)
        if(kalloc_list[i].task == t)
            kfree((void *)kalloc_list[i].ptr);
    task_unlock();
}
task *task_search_by_name(char *name){
    if(!name){
        KERROR("name == NULL");
        return 0x0;
    }
    task *t = task_first;
    while(t){
        if(strcmp(t->name, name) == 0)
            return t;
        t = t->next;
    }
    return 0x0;
}
void task_pause(task *t){
    if(!t){
        KERROR("t == NULL");
        return;
    }
    t->status = task_status_paused;
    INTERRUPT_TASK_PAUSE();
}
void task_resume(task *t){
    if(!t){
        KERROR("t == NULL");
        return;
    }
    t->status = task_status_running;
}
void task_allocation_add(kheap_allocated_block *b){
    task *task_branchless[] = { task_current, 0x0 };
    //KMESSAGE("task_new_memory_allocation == %d", (uint64_t)task_new_memory_allocation);
    task_lock();
    b->task = task_branchless[task_new_memory_allocation];
    task_unlock();
}
task *task_new(int(*fn)(void *, task *), void *data, char *name, task_priority priority){
    if(!fn){
        KERROR("fn == 0x0");
        return 0x0;
    }
    task_lock();
    task_new_memory_allocation = true;
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
        .priority = priority,
        .waiting_on = 0x0
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
        //kprintf("task_first == %s", ret->name);
        task_first = ret;
    }
    if(task_last){
        task_last->next = ret;
    }
    task_last = ret;
    task_new_memory_allocation = false;
    task_unlock();
    return ret;
}
task_priority task_priority_get(task *t){
    return t == 0x0 ? task_priority_null : t->priority;
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
    task_end_later(t);
    while (1){}
    
}
void task_scheduler(){
    rtc_device_time_since_boot_centisecond += task_current ? 1 : task_current->status == task_status_paused ? 0 : 1; //uglross
    task_scheduler_start:
    if(!task_first){
        KPANIC("task_first == NULL");
        return;
    }
    if(!task_current)
        task_current = task_first;
    
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
    if(!task_current){
        KPANIC("task_get_next() returned 0x0");
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
            KERROR("loaded 0x0 X_X");
            break;
        case task_status_ended:{
            task *tmpt = task_first;
            while(tmpt){
                if(tmpt->waiting_on == task_current)
                    task_resume(tmpt);
                tmpt = tmpt->next;
            }
            task_free(task_current);
            task_current = task_get_next();
            break;
        }
        case task_status_running:
            task_cpu_registers_reload(task_current->context);
            KERROR("reloaded 0x0 X_X");
            break;
        case task_status_wait_io:
            break;
        case task_status_paused:
            break;
        default:
            KPANIC("task_current (0x%x) :\n\ttask_current->status == %d (unknown)",
                task_current,
                task_current->status
            );
            break;
    }
    goto task_scheduler_start;
}
