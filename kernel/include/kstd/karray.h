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
#define _karray_contains(arr, e, fn)({\
    bool _karray_contains_return = false;\
    for(int _karray_contains_i = 0; _karray_contains_i < arr->length; _karray_contains_i++){\
        _karray_contains_return = fn(((typeof(e) *)arr->array)[_karray_contains_i], e);\
        if(_karray_contains_return)\
            break;\
    }\
    _karray_contains_return;\
})
#define _char_array_contains_static(arr, c) _char_array_contains_static_internal(arr, sizeof(arr), c)
static inline bool _char_array_contains_static_internal(const char arr[], int l, char c){
    for(int i = 0; i < l; i++)
        if(arr[i] == c)
            return true;
    return false;
}
#define _karray_static(arr) ({ \
    &(karray){\
        .elementsz = sizeof(arr[0]),\
        .length = sizeof(arr)/sizeof(arr[0]),\
        .alloc = sizeof(arr)/sizeof(arr[0]),\
        .karray_data_free_fn = 0x0,\
        .array = arr\
    };\
})
karray *karray_new(uint8_t elementsz, void(*karray_data_free_fn)(void *));
bool karray_contains_str(karray *array, char *e);
void karray_free(karray *array);
void karray_push(karray *array, uint64_t elem);
void karray_pop(karray *array ,uint64_t index);
void karray_debug_print(karray *array);
#include <kstd/kstd.h>
#endif
