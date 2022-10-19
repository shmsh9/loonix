#ifndef TASK_H_
#define TASK_H_
#include <stdint.h>
#include <arch/arch.h>

#define TASK_STACK_SIZE 0x1000

typedef struct  __attribute__((packed)) _task{
    struct _task *next;
    struct _task *prev;
    cpu_registers *context;
    void *stack;
    uint64_t uuid;
    uint8_t priority;
    char *name;
}task;
extern task *task_current;
extern task *task_first;

#include <kstd/kstd.h>

task *task_new(char *name);
task *task_get_next();
void task_free(task *t);
void task_debug_print();
#endif
