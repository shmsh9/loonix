#ifndef _VEC_H_
#define _VEC_H_
#include <stdint.h>

#define _VEC_FREE kfree
#define _VEC_MALLOC kmalloc
#define _VEC_REALLOC krealloc
#define TO_STR(E) "\"" #E "\""

#define VEC_BASE_SZ 8
typedef void* vec_ptr;
#define vec auto
#define vec_t(T) struct _vec_generic
#define vec_t_def(T) \
    vec_t(T){\
        T* _array;\
        uint64_t length;\
        uint64_t _alloc;\
    }
#define vec_init(v){\
    v->_array = _VEC_MALLOC(VEC_BASE_SZ*sizeof(v->_array[0]));\
    v->_alloc = VEC_BASE_SZ;\
    v->length = 0;\
}
#define vec_new(T)({\
    vec_t_def(T);\
    vec_t(T) *ret = _VEC_MALLOC(sizeof(vec_t(T)));\
    vec_init(ret);\
    ret;\
})
#define _vec_new_static(t, arr)({ \
    vec_t_def(t);\
    &(vec_t(t)){\
        ._array = arr,\
        .length = sizeof(arr)/sizeof(arr[0]),\
        ._alloc = 0\
    };\
})
#define vec_new_static(T, ...) _vec_new_static(T, ((T[])__VA_ARGS__))
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
        v->_array = _VEC_REALLOC(v->_array, v->_alloc*2*sizeof(v->_array[0]));\
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
#define _vec_ptr_iter(v,t,e,fn) vec_iter(vec_cast(v,t), e, fn)
#define vec_free(v) _VEC_FREE(v->_array); _VEC_FREE(v)
#define vec_free_obj(v) vec_iter(v, e, { kfree((void *)e); }); vec_free(v);
#define vec_map(v,e,fn)({\
    typeof(v) ret = _VEC_MALLOC(sizeof(*v));\
    vec_init(ret);\
    vec_iter(v,e, {\
        typeof(v->_array[0]) e2 = fn;\
        vec_push(ret, e2);\
    });\
    ret;\
})
#define vec_map_t(v,e,t,fn)({\
    vec ret = vec_new(t);\
    _vec_ptr_iter(v,t,e, {\
        t e2 = fn;\
        vec_push(ret, e2);\
    });\
    ret;\
})
#define vec_iter_next(v) vec_get(v, _vec_iter_i+1)
#define vec_iter_is_last(v)({ _vec_iter_i == v->length-1; })
#define vec_contains(v, e)({\
    bool ret = false;\
    vec_iter(v, x, { \
        if( x == e){\
            ret = true;\
            break;\
        }\
    })\
    ret;\
})
#define vec_where(v,e,fn)({ \
    typeof(v) ret = _VEC_MALLOC(sizeof(*v));\
    vec_init(ret);\
    vec_iter(v,e, { \
        bool r = fn;\
        if(r){\
            vec_push(ret, e);\
        }\
    });\
    ret;\
})
#define vec_print_fmt(v,fmt) \
    kprintf(TO_STR(v)" : {");\
    vec_iter(v, e, {\
        kprintf(fmt"%s", e, vec_iter_is_last(v) ? "}\n":", ");\
    });
#define vec_print_uint(v) vec_print_fmt(v, "%d")
#define vec_print_char(v) vec_print_fmt(v, "'%c'")
#define vec_print_str(v) vec_print_fmt(v, TO_STR(%s))
#endif
