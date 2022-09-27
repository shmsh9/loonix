#ifndef PROCESS_H_
#define PROCESS_H_
#include <kstd.h>
#include <arch/arch.h>
#define PROCESS_STACK_SIZE MB_TO_BYTES(1)
extern karray *process_list;
typedef struct{
    uint32_t id;
    uint8_t priority;
    void *stack;
    cpu_registers *registers;
    int argc;
    char **argv;
}process;

uint32_t process_get_last_id();
process *process_new(int (*fn)(int, char **), int argc, char **argv);
void process_karray_free(process *proc);
void process_init();
void process_free(uint32_t pid);
#endif
