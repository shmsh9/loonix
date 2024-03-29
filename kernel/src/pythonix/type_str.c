#include <pythonix/types.h>

void pythonix_type_str_free(pythonix_type *t){
    pythonix_type_str *s = (pythonix_type_str *)t->_data;
    kfree(s->data);
    kfree(s);
}
uint64_t pythonix_type_str__str__(pythonix_type *t, void *d){
    return (uint64_t)t->_data;
}
pythonix_type *pythonix_type_str_new(char *value, char *vname, pythonix_vm *vm){
    pythonix_type *ret = pythonix_type_new(PYTHONIX_TYPE_NAME_STR, vname, vm);
    pythonix_type_str *str = kmalloc(sizeof(pythonix_type_str));
    *str = (pythonix_type_str){
        .data = strdup(value),
        .length = strlen(value)
    };
    ret->_free = pythonix_type_str_free;
    ret->_data = (void *)str;
    pythonix_type_method_add(ret, pythonix_method_new("__str__", pythonix_type_str__str__));
    return ret;
}
