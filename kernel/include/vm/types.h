#ifndef VM_TYPES_H_
#define VM_TYPES_H_

#include <stdint.h>

typedef struct _vm_type{
    void *value;
    enum t {
        _vm_int,
        _vm_str,
        _vm_unknown
    } type;
} vm_type;

vm_type _get_type(uint8_t *in, uint64_t in_len);
#endif
