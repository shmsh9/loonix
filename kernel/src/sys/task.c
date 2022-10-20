#include <sys/task.h>

task *task_current = 0x0;
task *task_first = 0x0;
task *task_last = 0x0;

task *task_new(char *name){
    task *ret = kmalloc(sizeof(task));
    *ret = (task){
        .next = 0x0,
        .prev = task_last,  // task_last == 0x0 ? task_last : task_last;
        .uuid = cpu_get_tick(),
        .context = kcalloc(sizeof(cpu_registers),1),
        .stack = kmalloc(TASK_STACK_SIZE),
        .name = strdup(name)
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
    kfree(t->name);
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
        kprintf("\tTask %d : 0x%x (%s)\n", 
            i,
            t->uuid,
            t->name
        );
        CPU_REGISTERS_PRINT(t->context);
        i++;
        t = t->next;
    }
}

void task_scheduler(){
    if(!task_first)
        return;
    if(!task_current){
        task_current = task_first;
    }
    else{
        task_current = task_current->next == 0 ? task_first : task_current->next; 
    }
    KMESSAGE("task_current : %s", 
        task_current->name
    );
}
