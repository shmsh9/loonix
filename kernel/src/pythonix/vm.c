#include <pythonix/pythonix.h>


void pythonix_vm_free(pythonix_vm *vm){
    khashmap_free(vm->names);
    karray_free(vm->types);
    kfree(vm);
}
void pythonix_vm_gc(pythonix_vm *vm){
    for(int i = 0; i < vm->types->length; i++){
        pythonix_type *t = ((pythonix_type **)vm->types->array)[i];
        if(t->_ref_count == 0){
            khashmap_set(vm->names, t->_variable_name, 0x0);
            pythonix_type_free(t);
        }
    }
}
pythonix_type *pythonix_vm_get_type(pythonix_vm *vm, char *name){
    return (pythonix_type *)khashmap_get(vm->names, name);
}
pythonix_vm *pythonix_vm_new(){
    pythonix_vm *ret = kmalloc(sizeof(pythonix_vm));
    *ret = (pythonix_vm){
        .names = khashmap_new(),
        .types = karray_new(sizeof(pythonix_type*), (void (*)(void *))pythonix_type_free)
    };
    return ret;
}
