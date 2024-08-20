#ifndef _VEC_H_
#define _VEC_H_
#include <stdint.h>

#define TO_STR(E) "\"" #E "\""
#define VEC_BASE_SZ 8
typedef void* vec_ptr;
#define vec auto
#define vec_t(T) struct _vec_##T
#define vec_t_def(T) \
    vec_t(T){\
        T* _array;\
        uint64_t length;\
        uint64_t _alloc;\
    }
#define vec_new(T) ({ \
    vec_t_def(T);\
    vec_t(T) *ret = kmalloc(sizeof(vec_t(T)));\
    *ret = (vec_t(T)){\
        ._array = kmalloc(VEC_BASE_SZ * sizeof(T)),\
        .length = 0,\
        ._alloc = VEC_BASE_SZ\
    };\
    ret;\
})
#define vec_cast(V,T)({\
    vec_t_def(T);\
    vec_t(T) *ret = (vec_t(T)*)V;\
    ret;\
})
#define vec_push(v,e) \
    if(v->length+1 < v->_alloc){\
        v->_array[v->length++] = e;\
    } \
    else{ \
        v->_array = krealloc(v->_array, v->_alloc*2*sizeof(v->_array[0]));\
        v->_alloc *= 2;\
        v->_array[v->length++] = e;\
    }
#define vec_get(v,i)({\
    typeof(v->_array[0]) ret = 0x0;\
    if(v->length > i){\
        ret = v->_array[i];\
    }\
    ret;\
})
#define vec_iter(v,e,fn) \
    for(int i = 0; i < v->length; i++){ \
        int _vec_iter_i = i; \
        typeof(v->_array[0]) e = vec_get(v, i); \
        fn \
    }
#define vec_ptr_iter(v,t,e,fn) vec_iter(vec_cast(v,t), e, fn)
#define vec_free(v) kfree(v->_array); kfree(v)
#define vec_map(v,t,e,t2,e2,fn)({\
    vec ret = vec_new(t2);\
    vec_ptr_iter(v,t,e, {\
        t2 e2 = 0x0;\
        fn;\
        vec_push(ret, e2);\
    });\
    ret;\
})
#define vec_iter_next(v) vec_get(v, _vec_iter_i+1)
#define vec_ptr_iter_next(v, t) vec_get(vec_cast(v,t), _vec_iter_i+1)
#define vec_iter_is_last(v)({\
    _vec_iter_i == v->length-1;\
})
#define vec_ptr_is_last(v,t) vec_iter_is_last(vec_cast(v,t))
#define vec_print(v) \
    kprintf(TO_STR(v)" : {");\
    vec_iter(v, e, {\
        kprintf("%d%s", e, vec_iter_is_last(v) ? "}\n":", ");\
    });
#define vec_print_char(v) \
    kprintf(TO_STR(v)" : {");\
    vec_iter(v, e, {\
        kprintf("'%c'%s", e, vec_iter_is_last(v) ? "}\n":", ");\
    });
#endif
