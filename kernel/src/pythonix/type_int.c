#include <pythonix/types.h>

void pythonix_type_int__add__int(pythonix_type *self, pythonix_type *t2){
    self->_data = (void *)((int64_t)t2->_data+(int64_t)self->_data);
}
pythonix_type *pythonix_type_int__sub__(pythonix_type *self, void *d){
    pythonix_type *t2 = (pythonix_type *)d;
    if(!strcmp(t2->name, PYTHONIX_TYPE_NAME_INT)){
        self->_data = (void *)((int64_t)self->_data + ((int64_t)t2->_data)*-1);
        return self;
    }
    kprintf("TypeError: unsupported operand type(s) for +: '%s' and '%s'\n", self->name, t2->name);
    return 0x0;
}
pythonix_type *pythonix_type_int__mul__(pythonix_type *self, void *d){
    pythonix_type *t2 = (pythonix_type *)d;
    if(!strcmp(t2->name, PYTHONIX_TYPE_NAME_INT)){
        self->_data = (void *)((int64_t)self->_data * ((int64_t)t2->_data));
        return self;
    }
    kprintf("TypeError: unsupported operand type(s) for +: '%s' and '%s'\n", self->name, t2->name);
    return 0x0;
}
pythonix_type *pythonix_type_int__div__(pythonix_type *self, void *d){
    pythonix_type *t2 = (pythonix_type *)d;
    if(!strcmp(t2->name, PYTHONIX_TYPE_NAME_INT)){
        self->_data = (void *)((int64_t)self->_data / ((int64_t)t2->_data));
        return self;
    }
    kprintf("TypeError: unsupported operand type(s) for +: '%s' and '%s'\n", self->name, t2->name);
    return 0x0;
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
    int64_t v = (int64_t)self->_data;
    char *str = itoa(v);
    pythonix_type *ret = pythonix_type_str_new(str, PYTHONIX_VAR_NAME_ANON, (pythonix_vm *)self->_vm);
    kfree(str);
    return ret;
}
pythonix_type *pythonix_type_int__copy__(pythonix_type *self, void *d){
    char *copy_name = (char *)d;
    pythonix_type *copy = pythonix_type_int_new(
        (int64_t)self->_data,
        copy_name,
        (pythonix_vm *)self->_vm
    );
    return copy;
}
pythonix_type *pythonix_type_int_new(int64_t value, char *vname, pythonix_vm *vm){
    pythonix_type *ret = pythonix_type_new(PYTHONIX_TYPE_NAME_INT, vname, vm);
    ret->_str = pythonix_type_int__str__;
    ret->_copy = pythonix_type_int__copy__;
    pythonix_type_method_add(ret, pythonix_method_new("__add__", pythonix_type_int__add__));
    pythonix_type_method_add(ret, pythonix_method_new("__sub__", pythonix_type_int__sub__));
    pythonix_type_method_add(ret, pythonix_method_new("__mul__", pythonix_type_int__mul__));
    pythonix_type_method_add(ret, pythonix_method_new("__div__", pythonix_type_int__div__));

    ret->_data = (void *)value;
    return ret;
}
