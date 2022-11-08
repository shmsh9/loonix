#ifndef TASK_H_
#define TASK_H_
#include <stdint.h>
#include <arch/arch.h>
#include <kstd/karray.h>
#include <shell/shell.h>

#define TASK_STACK_SIZE 0x10000
#define TASK_CPU_TIME_TOTAL   0x100000
#define TASK_CPU_TIME_HIGH    0x2
#define TASK_CPU_TIME_MEDIUM  0x4
#define TASK_CPU_TIME_LOW     0x8
#define TASK_CPU_TIME_SLEEP   0xF

typedef enum _task_status {
    task_status_ended,
    task_status_running,
    task_status_created,
    task_status_wait_io
}task_status;

typedef enum _task_priority{
    task_priority_high,
    task_priority_medium,
    task_priority_low,
    task_priority_sleep
}task_priority;

typedef struct  __attribute__((packed)) _task{
    struct _task *next;
    struct _task *prev;
    cpu_registers *context; //do not change my order
    int(*fn)(void *, struct _task *);
    void *stack_start;
    void *stack_end;
    char *name;
    task_status status;
    void *data;
    uint32_t time_slice;
    int32_t time_slice_remaining;
    task_priority priority;
}task;

extern task *task_current;
extern task *task_first;

void task_lock();
void task_unlock();
void task_end(task *t);
void task_priority_set(task *t, task_priority p);
task_priority task_priority_get(task *t);
task_status task_status_get(task *t);
void task_end_wait(task *t);
task *task_new(int(*fn)(void *, task *), void *data, char *name, task_priority priority);
task *task_get_next();
void task_free(task *t);
void task_debug_print();
void task_scheduler();
void task_allocation_add(kheap_allocated_block *b);
void task_end_current();

#include <sys/newmem.h>
#include <kstd/kstd.h>

#endif
