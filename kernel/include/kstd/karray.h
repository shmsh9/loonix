#ifndef KARRAY_H_
#define KARRAY_H_
#include <stdint.h>
#include <stdbool.h>
typedef struct {
    uint8_t elementsz;
    uint32_t length;
    uint32_t alloc;
    int64_t lock;
    void(*karray_data_free_fn)(void *);
    void *array;
} karray;
#define _karray_contains(type, arr, e, fn)({\
    bool contains = false;\
    for(int i = 0; i < arr->length; i++){\
        contains = fn(((type *)arr->array)[i], e);\
        if(contains)\
            break;\
    }\
    contains;\
})
#define _karray_static(type, arr) ({ \
    type *stackarr[sizeof(arr)/sizeof(arr[0])] = {0};\
    karray *ret = &(karray){\
        .elementsz = sizeof(arr[0]),\
        .length = 0,\
        .alloc = sizeof(arr)/sizeof(arr[0]),\
        .karray_data_free_fn = 0x0,\
        .array = stackarr\
    }; \
    for(int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) \
        karray_push(ret, (uint64_t)arr[i]); \
    ret; \
})
karray *karray_new(uint8_t elementsz, void(*karray_data_free_fn)(void *));
bool karray_contains_str(karray *array, char *e);
void karray_free(karray *array);
void karray_push(karray *array, uint64_t elem);
void karray_pop(karray *array ,uint64_t index);
void karray_debug_print(karray *array);
#include <kstd/kstd.h>
#endif
