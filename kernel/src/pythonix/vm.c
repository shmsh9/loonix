#include <pythonix/pythonix.h>


void pythonix_vm_free(pythonix_vm *vm){
    khashmap_free(vm->names);
    kfree(vm);
}
pythonix_vm *pythonix_vm_new(){
    pythonix_vm *ret = kmalloc(sizeof(pythonix_vm));
    *ret = (pythonix_vm){
        .names = khashmap_new()
    };
    return ret;
}
