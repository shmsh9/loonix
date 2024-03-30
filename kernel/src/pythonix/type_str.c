#include <pythonix/types.h>

void pythonix_type_str_free(pythonix_type *t){
    pythonix_type_str *s = (pythonix_type_str *)t->_data;
    kfree(s->data);
    kfree(s);
}
void pythonix_type_str__add__str(pythonix_type_str *self, pythonix_type_str *t2){
    char *s = strings_join((char *[]){self->data, t2->data}, 2, 0x0);
    kfree(self->data);
    self->data = s;
    self->length = strlen(s);
}

pythonix_type *pythonix_type_str__add__(pythonix_type *self, void *d){
    pythonix_type *t2 = (pythonix_type *)d;
    if(!strcmp(t2->name, PYTHONIX_TYPE_NAME_STR)){
        pythonix_type_str__add__str((pythonix_type_str *)self->_data,(pythonix_type_str *)t2->_data);
        return self;
    }
    kprintf("TypeError: unsupported operand type(s) for +: '%s' and '%s'\n", self->name, t2->name);
    return 0x0;
}

pythonix_type *pythonix_type_str__str__(pythonix_type *self, void *d){
    pythonix_type *cp = pythonix_type_method_call(self, "__copy__", (void *)PYTHONIX_VAR_NAME_ANON);
    pythonix_type *ret = pythonix_type_str_new("'", PYTHONIX_VAR_NAME_ANON, (pythonix_vm *)self->_vm);
    pythonix_type_method_call(ret, "__add__", (void *)cp);
    pythonix_type_method_call(ret, "__add__", (void *)pythonix_type_str_new("'", PYTHONIX_VAR_NAME_ANON, (pythonix_vm *)self->_vm));
    return ret;
}
pythonix_type *pythonix_type_str__copy__(pythonix_type *self, void *d){
    char *copy_name = (char *)d;
    pythonix_type_str *s = ((pythonix_type_str *)self->_data);
    pythonix_type *copy = pythonix_type_str_new(
        s->data,
        copy_name,
        (pythonix_vm *)self->_vm
    );
    return copy;
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
    ret->_copy = pythonix_type_str__copy__;
    ret->_str = pythonix_type_str__str__;

    pythonix_type_method_add(ret, pythonix_method_new("__add__", pythonix_type_str__add__));
    return ret;
}
