#include <pythonix/types.h>

void pythonix_type_int__add__int(pythonix_type *self, pythonix_type *t2){
    self->_data = (void *)((int64_t)t2->_data+(int64_t)self->_data);
}
pythonix_type *pythonix_type_int__add__(pythonix_type *self, void *d){
    pythonix_type *t2 = (pythonix_type *)d;
    if(!strcmp(t2->name, PYTHONIX_TYPE_NAME_INT)){
        pythonix_type_int__add__int(self,t2);
        return self;
    }
    kprintf("TypeError: unsupported operand type(s) for +: '%s' and '%s'\n", self->name, t2->name);
    return 0x0;
}
pythonix_type *pythonix_type_int__str__(pythonix_type *self, void *d){
    char *name = strings_join(((char*[]){ self->_variable_name, "_str__"}), 2, '_');
    int64_t v = (int64_t)self->_data;
    char *str = itoa(v);
    pythonix_type *ret = pythonix_type_str_new(str, name, (pythonix_vm *)self->_vm);
    kfree(name);
    kfree(str);
    return ret;
}
pythonix_type *pythonix_type_int_new(int64_t value, char *vname, pythonix_vm *vm){
    pythonix_type *ret = pythonix_type_new(PYTHONIX_TYPE_NAME_INT, vname, vm);
    pythonix_type_method_add(ret, pythonix_method_new("__str__", pythonix_type_int__str__));
    pythonix_type_method_add(ret, pythonix_method_new("__add__", pythonix_type_int__add__));

    ret->_data = (void *)value;
    return ret;
}
