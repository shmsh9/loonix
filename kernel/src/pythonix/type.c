#include <pythonix/types.h>

void pythonix_method_free(pythonix_method *m){
    if(m){
        if(m->name)
            kfree(m->name);
        kfree(m);
    }
}
void pythonix_type_free(pythonix_type *t){
    if(t->_free)
        t->_free(t);
    if(t->_methods)
        karray_free(t->_methods);
    if(t->_methods_hashmap)
        khashmap_free(t->_methods_hashmap);
    if(t->name)
        kfree(t->name);
    if(t->_variable_name)
        kfree(t->_variable_name);
    kfree(t);
}
pythonix_type *pythonix_type__copy__(pythonix_type *t, void *p){
    if(!t){
        KERROR("t == NULL");
        return NULL;
    }
    if(!t->_copy){
        pythonix_type *copy = pythonix_type_new(
            t->name,
            (char *)p,
            (pythonix_vm *)t->_vm
        );
        copy->_data = t->_data;
        return copy;
    }
    else{
        return t->_copy(t,p);
    }
}
pythonix_type *pythonix_type__str__(pythonix_type *t, void *p){
    if(!t){
        KERROR("t == NULL");
        return NULL;
    }
    if(!t->_str){
        if(!t->name){
            KERROR("t->name == NULL");
            return NULL;
        }
        char *s = strings_join((char *[]){"<",t->name,">"},3,0x0);
        pythonix_type *str = pythonix_type_str_new(
            t->name,
            PYTHONIX_VAR_NAME_ANON,
            (pythonix_vm *)t->_vm
        );
        kfree(s);
        return str;
    }
    else{
        return t->_str(t,p);
    }
}

pythonix_method *pythonix_method_new(char *name, pythonix_type* (*m)(pythonix_type*,void*)){
    pythonix_method *ret = kmalloc(sizeof(pythonix_method));
    *ret = (pythonix_method){
        .name = strdup(name),
        ._method = m
    };
    return ret;
}
pythonix_type *pythonix_type_new(char *name, char *vname, pythonix_vm *vm){
    if(!vm){
        KERROR("vm == NULL");
        return NULL;
    }
    pythonix_type *ret = kmalloc(sizeof(pythonix_type));
    if(!ret){
        KERROR("ret == NULL");
        return NULL;
    }
    KDEBUG("%s (%s 0x%x)", vname ? vname : "NULL", name, ret);

    *ret = (pythonix_type){
        .name = strdup(name),
        ._methods = karray_new(sizeof(pythonix_method *), (void (*)(void *))pythonix_method_free),
        ._methods_hashmap = khashmap_new(),
        ._ref_count = vname == PYTHONIX_VAR_NAME_ANON ? 0 : 1,
        ._variable_name = vname == PYTHONIX_VAR_NAME_ANON ? PYTHONIX_VAR_NAME_ANON : strdup(vname),
        ._free = NULL,
        ._vm = vm,
        ._copy = NULL,
        ._str = NULL
    };
    pythonix_type_method_add(ret, pythonix_method_new("__copy__", pythonix_type__copy__));
    pythonix_type_method_add(ret, pythonix_method_new("__str__", pythonix_type__str__));
    karray_push(vm->types, (uint64_t)ret);
    if(vname){
        pythonix_type *old = pythonix_vm_get_type(vm, vname);
        if(old){
            old->_ref_count--;
            KDEBUG("replacing var %s (0x%x) with %s (0x%x)", old->_variable_name, old, ret->_variable_name,ret)
        }
        khashmap_set(vm->names, vname, (uint64_t)ret);
    }
    return ret;
}
pythonix_method *pythonix_type_method_get(pythonix_type *t, char *m){
    pythonix_method *ret = (pythonix_method *)khashmap_get(t->_methods_hashmap, m);
    return ret;
}

pythonix_type *pythonix_type_method_call(pythonix_type *self, char *m, void *p){
    if(!self){
        KERROR("self == NULL");
        return NULL;
    }
    pythonix_method *method = pythonix_type_method_get(self, m);
    if(!method){
        kprintf("AttributeError: '%s' object has no attribute '%s'\n", self->name, m);
        return 0;
    }
    return method->_method(self,p);
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

