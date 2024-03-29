#include <pythonix/types.h>

uint64_t pythonix_type_int__str__(pythonix_type *t, void *d){
    pythonix_vm *vm = (pythonix_vm *)d;
    char *name = strings_join(((char*[]){ t->name, "_str__"}), 2, '_');
    int64_t v = (int64_t)t->_data;
    int l = v > 0 ? 0 : 1; //account for -;
    int64_t v_tmp = v;
    while(v_tmp){
        v_tmp /= 10;
        l++;
    }
    char *str = kmalloc(sizeof(char)*(l+1));
    str[l] = 0x0;
    if(v < 0){
        str[0] = '-';
    }
    v_tmp = v;
    int i = l-1;
    while(v_tmp && i > -1){
        KDEBUG("str[%d] == %c", i, (v_tmp%10) + '0');
        str[i--] = (v_tmp%10) + '0';
        v_tmp /= 10;
    }
    pythonix_type *ret = pythonix_type_str_new(str, name, vm);
    kfree(name);
    kfree(str);
    return (uint64_t)ret->_data;
}
pythonix_type *pythonix_type_int_new(int64_t value, char *vname, pythonix_vm *vm){
    pythonix_type *ret = pythonix_type_new(PYTHONIX_TYPE_NAME_INT, vname, vm);
    pythonix_type_method_add(ret, pythonix_method_new("__str__", pythonix_type_int__str__));
    ret->_data = (void *)value;
    return ret;
}
