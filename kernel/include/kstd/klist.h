#ifndef KLIST_H_
#define KLIST_H_

#include <stdint.h>

typedef struct _klist{
    struct _klist_element *first;
    struct _klist_element *last;
    void(*klist_data_free_fn)(void *);
    int64_t lock;
} klist;

typedef struct _klist_element{
    struct _klist_element *next;
    struct _klist_element *prev;
    uintptr_t data;
}klist_element;

klist *klist_new(void(*klist_data_free_fn)(void *));
void klist_push(klist *k, uintptr_t data);
void klist_free(klist *k);
void klist_debug_print(klist *k);

#include <kstd/kstd.h>

#endif
