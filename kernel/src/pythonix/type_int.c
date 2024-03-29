#include <pythonix/types.h>

void pythonix_type_int__add__int(pythonix_type *t, pythonix_type *t2){
    t->_data = (void *)((int64_t)t2->_data+(int64_t)t->_data);
}
uint64_t pythonix_type_int__add__(pythonix_type *t, void *d){
    pythonix_type *t2 = (pythonix_type *)d;
    if(!strcmp(t2->name, PYTHONIX_TYPE_NAME_INT)){
        pythonix_type_int__add__int(t,t2);
        return (uint64_t)t;
    }
    kprintf("TypeError: unsupported operand type(s) for +: '%s' and '%s'\n", t->name, t2->name);
    return 0x0;
}
uint64_t pythonix_type_int__str__(pythonix_type *t, void *d){
    pythonix_vm *vm = (pythonix_vm *)d;
    char *name = strings_join(((char*[]){ t->_variable_name, t->name, "_str__"}), 3, '_');
    int64_t v = (int64_t)t->_data;
    char *str = itoa(v);
    pythonix_type *ret = pythonix_type_str_new(str, name, vm);
    kfree(name);
    kfree(str);
    return (uint64_t)ret->_data;
}
pythonix_type *pythonix_type_int_new(int64_t value, char *vname, pythonix_vm *vm){
    pythonix_type *ret = pythonix_type_new(PYTHONIX_TYPE_NAME_INT, vname, vm);
    pythonix_type_method_add(ret, pythonix_method_new("__str__", pythonix_type_int__str__));
    pythonix_type_method_add(ret, pythonix_method_new("__add__", pythonix_type_int__add__));

    ret->_data = (void *)value;
    return ret;
}
