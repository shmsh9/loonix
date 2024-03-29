#include <pythonix/types.h>

void pythonix_method_free(pythonix_method *m){
    kfree(m->name);
    m->name = 0x0;
    kfree(m);
}
void pythonix_type_free(pythonix_type *t){
    if(t->_free)
        t->_free(t);
    karray_free(t->_methods);
    khashmap_free(t->_methods_hashmap);
    kfree(t->name);
    kfree(t->_variable_name);
    kfree(t);
}
pythonix_method *pythonix_method_new(char *name, uint64_t (*m)(pythonix_type*,void*)){
    pythonix_method *ret = kmalloc(sizeof(pythonix_method));
    *ret = (pythonix_method){
        .name = strdup(name),
        ._method = m
    };
    return ret;
}
pythonix_type *pythonix_type_new(char *name, char *vname, pythonix_vm *vm){
    pythonix_type *old = pythonix_vm_get_type(vm, vname);
    if(old)
        old->_ref_count--;
    pythonix_type *ret = kmalloc(sizeof(pythonix_type));
    *ret = (pythonix_type){
        .name = strdup(name),
        ._methods = karray_new(sizeof(pythonix_method *), (void (*)(void *))pythonix_method_free),
        ._methods_hashmap = khashmap_new(),
        ._ref_count = 1,
        ._variable_name = strdup(vname),
        ._free = NULL
    };
    karray_push(vm->types, (uint64_t)ret);
    khashmap_set(vm->names, vname, (uint64_t)ret);
    return ret;
}
uint64_t pythonix_type_method_call(pythonix_type *t, char *m, void *p){
    pythonix_method *method = (pythonix_method *)khashmap_get(t->_methods_hashmap, m);
    if(!method){
        kprintf("AttributeError: '%s' object has no attribute '%s'", t->name, m);
        return 0;
    }
    return method->_method(t,p);
}
void pythonix_type_method_add(pythonix_type *t, pythonix_method *m){
    karray_push(
        t->_methods,
        (uint64_t)m
    );
    khashmap_set(
        t->_methods_hashmap,
        m->name,
        (uint64_t)m
    );
}

