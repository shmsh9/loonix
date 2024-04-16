#include <pythonix/pythonix.h>


void pythonix_vm_free(pythonix_vm *vm){
    khashmap_free(vm->names);
    karray_free(vm->types);
    kfree(vm);
}
void pythonix_vm_debug_print(pythonix_vm *vm){
    int l = vm->types->length;
    for(int i = 0; i < l; i++){
        pythonix_type *t = ((pythonix_type **)vm->types->array)[i];
        KDEBUG("%s (%s 0x%x) vm[%d] %d refs", t->_variable_name ? t->_variable_name : "NULL", t->name, t, i,t->_ref_count);
    }
}
void pythonix_vm_gc(pythonix_vm *vm){
    int l = vm->types->length;
    for(int i = l-1; i >= 0; i--){
        pythonix_type *t = ((pythonix_type **)vm->types->array)[i];
        if(t->_ref_count <= 0){
            KDEBUG("%s (%s 0x%x) vm[%d]", t->_variable_name ? t->_variable_name : "NULL", t->name, t, i);
            if(khashmap_get(vm->names, t->_variable_name) == (uint64_t)t)
                khashmap_set(vm->names, t->_variable_name, 0x0);
            pythonix_type_method_call(t, "__del__", NULL);
            karray_pop(vm->types, i);
        }
    }
}
pythonix_type *pythonix_vm_get_type(pythonix_vm *vm, char *name){
    if(!vm){
        KERROR("vm == NULL");
        return NULL;
    }
    return (pythonix_type *)khashmap_get(vm->names, name);
}
pythonix_vm *pythonix_vm_new(){
    pythonix_vm *ret = kmalloc(sizeof(pythonix_vm));
    *ret = (pythonix_vm){
        .names = khashmap_new(),
        .types = karray_new(sizeof(pythonix_type*), NULL)
    };
    return ret;
}

