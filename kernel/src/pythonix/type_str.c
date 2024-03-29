#include <pythonix/types.h>

void pythonix_type_str_free(pythonix_type *t){
    pythonix_type_str *s = (pythonix_type_str *)t->_data;
    kfree(s->data);
    kfree(s);
}
void pythonix_type_str__add__str(pythonix_type_str *t, pythonix_type_str *t2){
    char *s = strings_join((char *[]){t->data, t2->data}, 2, 0x0);
    kfree(t->data);
    t->data = s;
    t->length = strlen(s);
}

uint64_t pythonix_type_str__add__(pythonix_type *t, void *d){
    pythonix_type *t2 = (pythonix_type *)d;
    if(!strcmp(t2->name, PYTHONIX_TYPE_NAME_STR)){
        pythonix_type_str__add__str((pythonix_type_str *)t->_data,(pythonix_type_str *)t2->_data);
        return (uint64_t)t;
    }
    kprintf("TypeError: unsupported operand type(s) for +: '%s' and '%s'\n", t->name, t2->name);
    return 0x0;
}

uint64_t pythonix_type_str__str__(pythonix_type *t, void *d){
    pythonix_vm *vm = (pythonix_vm *)d;
    pythonix_type_str *slf = (pythonix_type_str *)t->_data;
    char *name = strings_join(((char*[]){ t->_variable_name, t->name, "_str__"}), 3, '_');
    char *str = strings_join(((char*[]){ "'", slf->data, "'"}), 3, 0x0);
    pythonix_type *ret = pythonix_type_str_new(str, name, vm);
    kfree(name);
    kfree(str);
    return (uint64_t)ret->_data;
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
    pythonix_type_method_add(ret, pythonix_method_new("__add__", pythonix_type_str__add__));

    return ret;
}
