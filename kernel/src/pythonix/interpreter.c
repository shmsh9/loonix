#include <pythonix/pythonix.h>
void pythonix_assign_int(karray *a, pythonix_vm *vm){
    int64_t value = atoi(((char **)a->array)[PYTHONIX_REGEX_ASSIGN_INT_VAL_GROUP]);
    char *name =  ((char **)a->array)[PYTHONIX_REGEX_ASSIGN_INT_NAME_GROUP];
    pythonix_type_int_new(value, name, vm);
}
void pythonix_assign_var(karray *a, pythonix_vm *vm){
    //KDEBUG("called with a == 0x%x", a);
}
void pythonix_assign_str(karray *a, pythonix_vm *vm){
    char *name =  ((char **)a->array)[PYTHONIX_REGEX_ASSIGN_STR_NAME_GROUP];
    char *value =  ((char **)a->array)[PYTHONIX_REGEX_ASSIGN_STR_VAL_GROUP];
    pythonix_type_str_new(value, name, vm);
}
void pythonix_print_var(karray *a, pythonix_vm *vm){
    //KDEBUG("called with a == 0x%x", a);
    char *name = ((char **)a->array)[PYTHONIX_REGEX_PRINT_VAR_NAME_GROUP];
    pythonix_type *t = pythonix_vm_get_type(vm, name);
    if(!t){
        kprintf("NameError: name '%s' is not defined\n", name);
        return;
    }
    pythonix_type_str *str = (pythonix_type_str *)pythonix_type_method_call(t, "__str__", (void *)vm);
    if(str)
        kprintf("%s\n", str->data);
}
void pythonix_var_add_int(karray *a, pythonix_vm *vm){
    char *name =  ((char **)a->array)[PYTHONIX_REGEX_VAR_ADD_INT_NAME_GROUP];
    char *value =  ((char **)a->array)[PYTHONIX_REGEX_VAR_ADD_INT_VAL_GROUP];
    pythonix_type *t = pythonix_vm_get_type(vm, name);
    if(!t){
        kprintf("NameError: name '%s' is not defined\n", name);
        return;
    }
    char *tmps = strings_join((char*[]){name, "_add_", value}, 2, '_');
    pythonix_type *i = pythonix_type_int_new(atoi(value), tmps, vm);
    pythonix_type_method_call(t, "__add__", (void *)i);
    i->_ref_count--;

}
void pythonix_var_add_str(karray *a, pythonix_vm *vm){
    char *name =  ((char **)a->array)[PYTHONIX_REGEX_VAR_ADD_STR_NAME_GROUP];
    char *value =  ((char **)a->array)[PYTHONIX_REGEX_VAR_ADD_STR_VAL_GROUP];
    pythonix_type *t = pythonix_vm_get_type(vm, name);
    if(!t){
        kprintf("NameError: name '%s' is not defined\n", name);
        return;
    }
    char *tmps = strings_join((char*[]){name, "_add_", value}, 2, '_');
    pythonix_type *s = pythonix_type_str_new(value, tmps, vm);
    pythonix_type_method_call(t, "__add__", (void *)s);
    kfree(tmps);
    s->_ref_count--;
}
void pythonix_not_impl(karray *a, pythonix_vm *vm){
    kprintf("NotImplError: Function not implemented\n");
}
void pythonix_interpreter_init(pythonix_vm *vm){

}
void pythonix_interpreter(){
    uint64_t _pythonix_actions_regex[][2] = {
        [PYTHONIX_ASSIGN_STR ]={(uint64_t)"PYTHONIX_REGEX_ASSIGN_STR", (uint64_t)regex_new(PYTHONIX_REGEX_ASSIGN_STR)},
        [PYTHONIX_ASSIGN_INT ]={(uint64_t)"PYTHONIX_REGEX_ASSIGN_INT", (uint64_t)regex_new(PYTHONIX_REGEX_ASSIGN_INT)},
        [PYTHONIX_ASSIGN_VAR ]={(uint64_t)"PYTHONIX_REGEX_ASSIGN_VAR", (uint64_t)regex_new(PYTHONIX_REGEX_ASSIGN_VAR)},
        [PYTHONIX_VAR_ADD_INT]={(uint64_t)"PYTHONIX_REGEX_ADD_INT",(uint64_t)regex_new(PYTHONIX_REGEX_VAR_ADD_INT)},
        [PYTHONIX_VAR_ADD_STR]={(uint64_t)"PYTHONIX_REGEX_ADD_STR",(uint64_t)regex_new(PYTHONIX_REGEX_VAR_ADD_STR)},
        [PYTHONIX_PRINT_VAR  ]={(uint64_t)"PYTHONIX_REGEX_PRINT_VAR", (uint64_t)regex_new(PYTHONIX_REGEX_PRINT_VAR)},
        [PYTHONIX_FN_CALL    ]={(uint64_t)"PYTHONIX_REGEX_FN_CALL", (uint64_t)regex_new(PYTHONIX_REGEX_FN_CALL)},
        [PYTHONIX_FN_DEF     ]={(uint64_t)"PYTHONIX_REGEX_FN_DEF", (uint64_t)regex_new(PYTHONIX_REGEX_FN_DEF)}
    };
    uint64_t _pythonix_actions_pointers[sizeof(_pythonix_actions_regex)/sizeof(_pythonix_actions_regex[0])] = {
        [PYTHONIX_ASSIGN_STR ]=(uint64_t)pythonix_assign_str,
        [PYTHONIX_ASSIGN_INT ]=(uint64_t)pythonix_assign_int,
        [PYTHONIX_ASSIGN_VAR ]=(uint64_t)pythonix_not_impl,
        [PYTHONIX_VAR_ADD_INT]=(uint64_t)pythonix_var_add_int,
        [PYTHONIX_VAR_ADD_STR]=(uint64_t)pythonix_var_add_str,
        [PYTHONIX_PRINT_VAR  ]=(uint64_t)pythonix_print_var,
        [PYTHONIX_FN_CALL    ]=(uint64_t)pythonix_not_impl,
        [PYTHONIX_FN_DEF     ]=(uint64_t)pythonix_not_impl
    };
    kprint(
        "Pythonix 0.0.1 on loonix\n"
        "Type \"help\", \"copyright\", \"credits\" or \"license\" for more information."
    );
    pythonix_vm *vm = pythonix_vm_new();
    pythonix_interpreter_init(vm);
    karray *in = karray_new(sizeof(char), NULL);
    kprintf(">>> ");
    while(true){
        char c = kgetchar_non_blocking();
        while(!c){
            c = kgetchar_non_blocking();
        }
        c = c == '\r' ? '\n' : c;
        karray_push(in, c);
        kprintf("%c", c);
        if(c == '\n'){
            char *line = kmalloc(sizeof(char)*(in->length+1));
            line[in->length] = 0x0;
            memcpy(line, in->array, in->length);
            bool m = false;
            for(int i = 0; i < sizeof(_pythonix_actions_regex)/sizeof(_pythonix_actions_regex[0]); i++){
                karray *exp = (karray *)_pythonix_actions_regex[i][1];
                if(regex_match(exp, line)){
                    karray *matches = regex_match_group(exp, line);
                    if(matches){
                        m = true;
                        karray *groups = regex_group_join(matches);
                        ((void(*)(karray *a, pythonix_vm *vm))_pythonix_actions_pointers[i])(groups, vm);
                        karray_free(groups);
                        karray_free(matches);
                    }
                    else{
                        KERROR("regex_match(exp, s) == true && regex_match_groups(exp, s) == NULL");
                    }
                }
            }
            if(!m){
                kprintf("error invalid syntax : %s\n", line);
                //for(int i = 0; i < in->length; i++)
                //    KDEBUG("in[%d] == %c",i,((char *)in->array)[i]);
            }
            kfree(line);
            karray_clear(in);
            pythonix_vm_gc(vm);
            kprintf(">>> ");
        }
    }
    pythonix_vm_free(vm);
    for(int i = 0; i < sizeof(_pythonix_actions_regex)/sizeof(_pythonix_actions_regex[0]); i++)
        karray_free((karray *)_pythonix_actions_regex[i][1]);
}
