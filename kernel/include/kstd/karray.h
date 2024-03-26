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
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#define _array_len(arr) (sizeof(arr)/sizeof(arr[0]))
#define _array_contains_1(arr, e)  (arr[0 ] == e)
#define _array_contains_2(arr, e)  (arr[1 ] == e) || _array_contains_1(arr, e)
#define _array_contains_3(arr, e)  (arr[2 ] == e) || _array_contains_2(arr, e)
#define _array_contains_4(arr, e)  (arr[3 ] == e) || _array_contains_3(arr, e)
#define _array_contains_5(arr, e)  (arr[4 ] == e) || _array_contains_4(arr, e)
#define _array_contains_6(arr, e)  (arr[5 ] == e) || _array_contains_5(arr, e)
#define _array_contains_7(arr, e)  (arr[6 ] == e) || _array_contains_6(arr, e)
#define _array_contains_8(arr, e)  (arr[7 ] == e) || _array_contains_7(arr, e)
#define _array_contains_9(arr, e)  (arr[8 ] == e) || _array_contains_8(arr, e)
#define _array_contains_10(arr, e) (arr[9 ] == e) || _array_contains_9(arr, e)
#define _array_contains_11(arr, e) (arr[10] == e) || _array_contains_10(arr, e)
#define _array_contains_12(arr, e) (arr[11] == e) || _array_contains_11(arr, e)
#define _array_contains_13(arr, e) (arr[12] == e) || _array_contains_12(arr, e)
#define _array_contains_14(arr, e) (arr[13] == e) || _array_contains_13(arr, e)
#define _array_contains_15(arr, e) (arr[14] == e) || _array_contains_14(arr, e)
#define _array_contains_16(arr, e) (arr[15] == e) || _array_contains_15(arr, e)
#define _array_contains_17(arr, e) (arr[16] == e) || _array_contains_16(arr, e)
#define _array_contains_18(arr, e) (arr[17] == e) || _array_contains_17(arr, e)
#define _array_contains_19(arr, e) (arr[18] == e) || _array_contains_18(arr, e)
#define _array_contains_20(arr, e) (arr[19] == e) || _array_contains_19(arr, e)
#define _array_contains_21(arr, e) (arr[20] == e) || _array_contains_20(arr, e)
#define _array_contains_22(arr, e) (arr[21] == e) || _array_contains_21(arr, e)
#define _array_contains_23(arr, e) (arr[22] == e) || _array_contains_22(arr, e)

#define _array_contains(arr, e)({\
    bool _r = false;\
    switch (sizeof(arr)/sizeof(arr[0])){\
    case 1:\
        _r = _array_contains_1(arr, e);\
        break;\
    case 2: \
        _r = _array_contains_2(arr, e);\
        break;\
    case 3: \
        _r = _array_contains_3(arr, e);\
        break;\
    case 4: \
        _r = _array_contains_4(arr, e);\
        break;\
    case 5: \
        _r = _array_contains_5(arr, e);\
        break;\
    case 6: \
        _r = _array_contains_6(arr, e);\
        break;\
    case 7: \
        _r = _array_contains_7(arr, e);\
        break;\
    case 8: \
        _r = _array_contains_8(arr, e);\
        break;\
    case 9: \
        _r = _array_contains_9(arr, e);\
        break;\
    default: \
        break;\
    }\
    _r;\
})
#define _karray_contains_fn(arr, e, fn)({\
    bool _karray_contains_return = false;\
    for(int _karray_contains_i = 0; _karray_contains_i < arr->length; _karray_contains_i++){\
        _karray_contains_return = fn(((typeof(e) *)arr->array)[_karray_contains_i], e);\
        if(_karray_contains_return)\
            break;\
    }\
    _karray_contains_return;\
})
#define _karray_contains(arr, e)({\
    bool _karray_contains_return = false;\
    for(int _karray_contains_i = 0; _karray_contains_i < arr->length; _karray_contains_i++){\
        _karray_contains_return = ((typeof(e) *)arr->array)[_karray_contains_i] == e;\
        if(_karray_contains_return)\
            break;\
    }\
    _karray_contains_return;\
})
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
void karray_free(karray *array);
void karray_push(karray *array, uint64_t elem);
void karray_pop(karray *array ,uint64_t index);
void karray_debug_print(karray *array);
#include <kstd/kstd.h>
#endif
