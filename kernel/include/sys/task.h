#ifndef TASK_H_
#define TASK_H_
#include <stdint.h>
#include <arch/arch.h>

#define TASK_STACK_SIZE 0x1000
#define TASK_FUNCTION __attribute__((noreturn))
#define TASK_FUNCTION_END(t){\
    t->status = task_status_ended;\
    while(1){}\
}
typedef enum _task_status {
    task_status_ended,
    task_status_running,
    task_status_created
}task_status;

typedef struct  __attribute__((packed)) _task{
    struct _task *next;
    struct _task *prev;
    cpu_registers *context;
    void(*fn)(void *, struct _task *);
    void *stack_start;
    void *stack_end;
    task_status status;
    uint64_t uuid;
    uint8_t priority;
    void *data;
}task;

extern task *task_current;
extern task *task_first;
extern void *task_kernel_stack;
#include <kstd/kstd.h>

void task_end(task *t);
task *task_new(void(*fn)(void *, task *), void *data);
task *task_get_next();
void task_free(task *t);
void task_debug_print();
void task_scheduler();
#endif
