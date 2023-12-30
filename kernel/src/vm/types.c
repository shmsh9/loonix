#include <vm/types.h>
#include <kstd/kstd.h>

vm_type _get_type_str(uint8_t *in, uint64_t in_len){
    return (vm_type){
        .value = 0x0,
        .type = _vm_str
    };
}

vm_type _get_type_int(uint8_t *in, uint64_t in_len){
    return (vm_type){
        .value = 0x0,
        .type = _vm_int
    };
}


vm_type _get_type(uint8_t *in, uint64_t in_len){
    vm_type (*f[256])(uint8_t *, uint64_t) = {
        ['\''] = _get_type_str,
        ['\"'] = _get_type_str,
        ['0' ] = _get_type_int,
        ['1' ] = _get_type_int,
        ['2' ] = _get_type_int,
        ['3' ] = _get_type_int,
        ['4' ] = _get_type_int,
        ['5' ] = _get_type_int,
        ['6' ] = _get_type_int,
        ['7' ] = _get_type_int,
        ['8' ] = _get_type_int,
        ['9' ] = _get_type_int
    };
    switch ((uint64_t)f[in[0]]){
        case 0:
            return (vm_type){
                .value = 0x0,
                .type = _vm_unknown
            };
        break;

        default:
            {
                return f[in[0]](in, in_len);
                break;
            }
    }

}
