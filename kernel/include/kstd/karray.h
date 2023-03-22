#ifndef KARRAY_H_
#define KARRAY_H_
#include <stdint.h>
typedef struct {
    uint8_t elementsz;
    uint32_t length;
    uint32_t alloc;
    int64_t lock;
    void(*karray_data_free_fn)(void *);
    void *array;
} karray;
karray *karray_new(uint8_t elementsz, void(*karray_data_free_fn)(void *));
void karray_free(karray *array);
void karray_push(karray *array, uint64_t elem);
void karray_pop(karray *array ,uint64_t index);
void karray_debug_print(karray *array);
#include <kstd/kstd.h>
#endif
