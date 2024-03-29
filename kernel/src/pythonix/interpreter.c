#include <pythonix/pythonix.h>
void pythonix_not_impl(karray *a, pythonix_vm *vm){
    kprintf("NotImplError: Function not implemented\n");
}
void inline static pythonix_op_find(uint64_t actions[], uint64_t regex[][2], uint32_t l, char *line, pythonix_vm *vm){
    bool m = false;
    for(int i = 0; i < l; i++){
        if(!regex[i][0])
            regex[i][0] = (uint64_t)regex_new((char *)regex[i][1]);
        karray *exp = (karray *)regex[i][0];
        if(regex_match(exp, line)){
            karray *matches = regex_match_group(exp, line);
            if(matches){
                if(m){
                    KDEBUG("matched more than once regex[%d]", i);
                }
                m = true;
                karray *groups = regex_group_join(matches);
                ((void(*)(karray *a, pythonix_vm *vm))actions[i])(groups, vm);
                karray_free(groups);
                karray_free(matches);
                return;
            }
            else{
                KERROR("regex_match(exp, s) == true && regex_match_groups(exp, s) == NULL");
            }
        }
    }
    if(!m)
        kprintf("error invalid syntax : %s", line);
    //for(int i = 0; i < l; i++)
    //    karray_free((karray *)regex[i]);
}
void pythonix_assign_int(karray *a, pythonix_vm *vm){
    int64_t value = atoi(((char **)a->array)[PYTHONIX_REGEX_ASSIGN_INT_VAL_GROUP]);
    char *name =  ((char **)a->array)[PYTHONIX_REGEX_ASSIGN_INT_NAME_GROUP];
    pythonix_type_int_new(value, name, vm);
}
void pythonix_assign_var(karray *a, pythonix_vm *vm){
    char *name = ((char **)a->array)[PYTHONIX_REGEX_ASSIGN_VAR_NAME_GROUP];
    char *value =  ((char **)a->array)[PYTHONIX_REGEX_ASSIGN_VAR_VAL_GROUP];
    pythonix_type *t2 = pythonix_vm_get_type(vm, value);
    if(!t2){
        kprintf("NameError: name '%s' is not defined\n", value);
        return;
    }
    pythonix_type_method_call(
        t2,
        "__copy__",
        (void *)name
    );
}
void pythonix_assign_str(karray *a, pythonix_vm *vm){
    char *name = ((char **)a->array)[PYTHONIX_REGEX_ASSIGN_STR_NAME_GROUP];
    char *value =  ((char **)a->array)[PYTHONIX_REGEX_ASSIGN_STR_VAL_GROUP];
    char *n1 = string_remove(value, '\'');
    char *n2 = string_remove(n1, '"');
    kfree(n1);
    pythonix_type_str_new(n2, name, vm);
    kfree(n2);
}
uint64_t _pythonix_assign_actions_regex[][2] = {
    [PYTHONIX_ASSIGN_STR ]= {0, (uint64_t)PYTHONIX_REGEX_ASSIGN_STR},
    [PYTHONIX_ASSIGN_INT ]= {0, (uint64_t)PYTHONIX_REGEX_ASSIGN_INT},
    [PYTHONIX_ASSIGN_VAR ]= {0, (uint64_t)PYTHONIX_REGEX_ASSIGN_VAR}
};
void pythonix_assign_any(karray *a, pythonix_vm *vm){
    uint64_t _pythonix_assign_actions_pointers[sizeof(_pythonix_assign_actions_regex)/sizeof(_pythonix_assign_actions_regex[0])] = {
        [PYTHONIX_ASSIGN_STR ]=(uint64_t)pythonix_assign_str,
        [PYTHONIX_ASSIGN_INT ]=(uint64_t)pythonix_assign_int,
        [PYTHONIX_ASSIGN_VAR ]=(uint64_t)pythonix_assign_var,
    };
    pythonix_op_find(
        _pythonix_assign_actions_pointers, 
        _pythonix_assign_actions_regex,
        sizeof(_pythonix_assign_actions_regex)/sizeof(_pythonix_assign_actions_regex[0]),
        ((char **)a->array)[0],
        vm
    );
}
void pythonix_print_var(karray *a, pythonix_vm *vm){
    //KDEBUG("called with a == 0x%x", a);
    char *name = ((char **)a->array)[PYTHONIX_REGEX_PRINT_VAR_NAME_GROUP];
    pythonix_type *t = pythonix_vm_get_type(vm, name);
    if(!t){
        kprintf("NameError: name '%s' is not defined\n", name);
        return;
    }
    pythonix_type *str = pythonix_type_method_call(t, "__str__", (void *)vm);
    if(str && !strcmp(PYTHONIX_TYPE_NAME_STR, str->name))
        kprintf("%s\n", ((pythonix_type_str *)(str->_data))->data);
}
void pythonix_var_add_str(karray *a, pythonix_vm *vm){
    char *name =  ((char **)a->array)[PYTHONIX_REGEX_VAR_ADD_STR_NAME_GROUP];
    char *value =  ((char **)a->array)[PYTHONIX_REGEX_VAR_ADD_STR_VAL_GROUP];
    pythonix_type *t = pythonix_vm_get_type(vm, name);
    if(!t){
        kprintf("NameError: name '%s' is not defined\n", name);
        return;
    }
    char *n1 = string_remove(value, '\'');
    char *n2 = string_remove(n1, '"');
    kfree(n1);
    char *tmps = strings_join((char*[]){name, "_add_", value}, 3, '_');
    pythonix_type *i = pythonix_type_str_new(n2, tmps, vm);
    kfree(n2);
    kfree(tmps);
    pythonix_type_method_call(t, "__add__", (void *)i);
    i->_ref_count--;
}
void pythonix_var_add_int(karray *a, pythonix_vm *vm){
    char *name =  ((char **)a->array)[PYTHONIX_REGEX_VAR_ADD_INT_NAME_GROUP];
    char *value =  ((char **)a->array)[PYTHONIX_REGEX_VAR_ADD_INT_VAL_GROUP];
    pythonix_type *t = pythonix_vm_get_type(vm, name);
    if(!t){
        kprintf("NameError: name '%s' is not defined\n", name);
        return;
    }
    char *tmps = strings_join((char*[]){name, "_add_", value}, 3, '_');
    pythonix_type *i = pythonix_type_int_new(atoi(value), tmps, vm);
    pythonix_type_method_call(t, "__add__", (void *)i);
    i->_ref_count--;
}
uint64_t _pythonix_var_operator_add_actions_regex[][2] = {
    [PYTHONIX_VAR_OPERATOR_ADD_STR ]= {0, (uint64_t)(PYTHONIX_REGEX_VAR_ADD_STR)},
    [PYTHONIX_VAR_OPERATOR_ADD_INT ]= {0, (uint64_t)(PYTHONIX_REGEX_VAR_ADD_INT)},
    [PYTHONIX_VAR_OPERATOR_ADD_VAR ]= {0, (uint64_t)(PYTHONIX_REGEX_VAR_ADD_VAR)}
};
void pythonix_var_operator_add(karray *a, pythonix_vm *vm){
    uint64_t _pythonix_var_operator_add_actions_pointers[sizeof(_pythonix_var_operator_add_actions_regex)/sizeof(_pythonix_var_operator_add_actions_regex[0])] = {
        [PYTHONIX_VAR_OPERATOR_ADD_STR ]=(uint64_t)pythonix_var_add_str,
        [PYTHONIX_VAR_OPERATOR_ADD_INT ]=(uint64_t)pythonix_var_add_int,
        [PYTHONIX_VAR_OPERATOR_ADD_VAR ]=(uint64_t)pythonix_not_impl,
    };
    pythonix_op_find(
        _pythonix_var_operator_add_actions_pointers, 
        _pythonix_var_operator_add_actions_regex,
        sizeof(_pythonix_var_operator_add_actions_regex)/sizeof(_pythonix_var_operator_add_actions_regex[0]),
        ((char **)a->array)[0],
        vm
    );

}
uint64_t _pythonix_var_operator_actions_regex[][2] = {
    [PYTHONIX_VAR_OPERATOR_ADD ]= {0, (uint64_t)(PYTHONIX_REGEX_VAR_OPERATOR_ADD)},
    [PYTHONIX_VAR_OPERATOR_SUB ]= {0, (uint64_t)(PYTHONIX_REGEX_VAR_OPERATOR_SUB)},
    [PYTHONIX_VAR_OPERATOR_MUL ]= {0, (uint64_t)(PYTHONIX_REGEX_VAR_OPERATOR_MUL)},
    [PYTHONIX_VAR_OPERATOR_DIV ]= {0, (uint64_t)(PYTHONIX_REGEX_VAR_OPERATOR_DIV)}
};
void pythonix_var_operator_any(karray *a, pythonix_vm *vm){
    uint64_t _pythonix_var_operator_actions_pointers[sizeof(_pythonix_var_operator_actions_regex)/sizeof(_pythonix_var_operator_actions_regex[0])] = {
        [PYTHONIX_VAR_OPERATOR_ADD ]=(uint64_t)pythonix_var_operator_add,
        [PYTHONIX_VAR_OPERATOR_SUB ]=(uint64_t)pythonix_not_impl,
        [PYTHONIX_VAR_OPERATOR_MUL ]=(uint64_t)pythonix_not_impl,
        [PYTHONIX_VAR_OPERATOR_DIV ]=(uint64_t)pythonix_not_impl
    };
    pythonix_op_find(
        _pythonix_var_operator_actions_pointers, 
        _pythonix_var_operator_actions_regex,
        sizeof(_pythonix_var_operator_actions_regex)/sizeof(_pythonix_var_operator_actions_regex[0]),
        ((char **)a->array)[0],
        vm
    );


}
void pythonix_method_call(karray *a, pythonix_vm *vm){
    char *name =  ((char **)a->array)[PYTHONIX_REGEX_METHOD_CALL_VAR_NAME_GROUP];
    char *method =  ((char **)a->array)[PYTHONIX_REGEX_METHOD_CALL_METHOD_NAME_GROUP];
    pythonix_type *t = pythonix_vm_get_type(vm, name);
    if(!t){
        kprintf("NameError: name '%s' is not defined\n", name);
        return;
    }
    pythonix_type *t2 = (pythonix_type *)pythonix_type_method_call(t, method, (void *)vm);
    if(!strcmp(t->name, PYTHONIX_TYPE_NAME_STR) && !strcmp(method, "__str__")){
        kprintf("%s\n", ((pythonix_type_str *)(t2->_data))->data);
        t2->_ref_count--;
        return;
    }
    if(t2){
        pythonix_method *has_str = pythonix_type_method_get(t2, "__str__");
        if(has_str){
            pythonix_type *str = (pythonix_type *)pythonix_type_method_call(t2, "__str__", 0x0);
            kprintf("%s\n", ((pythonix_type_str *)(str->_data))->data);
            str->_ref_count--;
        }
        t2->_ref_count--;
    }
}
pythonix_type *pythonix_global_help__str__(pythonix_type *t, void *vm){
    char *name = strings_join(((char*[]){ t->_variable_name, t->name, "_str__"}), 3, '_');
    pythonix_type *ret = pythonix_type_str_new(
        "Type help() for interactive help, or help(object) for help about object.", 
        name, 
        (pythonix_vm *)vm
    );
    kfree(name);
    return ret;
}
void pythonix_interpreter_init(pythonix_vm *vm){
    pythonix_type *help = pythonix_type_new("global", "help", vm);
    pythonix_type_method_add(help, pythonix_method_new("__str__", pythonix_global_help__str__));
}
void pythonix_interpreter(){
    uint64_t _pythonix_actions_regex[] = {
        [PYTHONIX_ASSIGN_ANY      ]=(uint64_t)regex_new(PYTHONIX_REGEX_ASSIGN_ANY),
        [PYTHONIX_PRINT_VAR       ]=(uint64_t)regex_new(PYTHONIX_REGEX_PRINT_VAR),
        [PYTHONIX_FN_CALL         ]=(uint64_t)regex_new(PYTHONIX_REGEX_FN_CALL),
        [PYTHONIX_FN_DEF          ]=(uint64_t)regex_new(PYTHONIX_REGEX_FN_DEF),
        [PYTHONIX_METHOD_CALL     ]=(uint64_t)regex_new(PYTHONIX_REGEX_METHOD_CALL),
        [PYTHONIX_VAR_OPERATOR_ANY]=(uint64_t)regex_new(PYTHONIX_REGEX_VAR_OPERATOR_ANY),

    };
    uint64_t _pythonix_actions_pointers[sizeof(_pythonix_actions_regex)/sizeof(_pythonix_actions_regex[0])] = {
        [PYTHONIX_ASSIGN_ANY       ]=(uint64_t)pythonix_assign_any,
        [PYTHONIX_PRINT_VAR        ]=(uint64_t)pythonix_print_var,
        [PYTHONIX_FN_CALL          ]=(uint64_t)pythonix_not_impl,
        [PYTHONIX_FN_DEF           ]=(uint64_t)pythonix_not_impl,
        [PYTHONIX_METHOD_CALL      ]=(uint64_t)pythonix_method_call,
        [PYTHONIX_VAR_OPERATOR_ANY ]=(uint64_t)pythonix_var_operator_any

    };
    kprint(
        "Pythonix 0.0.1 on loonix\n"
        "Type \"help\", \"copyright\", \"credits\" or \"license\" for more information.\n"
    );
    pythonix_vm *vm = pythonix_vm_new();
    pythonix_interpreter_init(vm);
    karray *in = karray_new(sizeof(char), NULL);
    kprintf(">>> ");
    while(true){
        vt100_console_update_draw_screen(fb);
        char c = kgetchar_non_blocking();
        while(!c){
            c = kgetchar_non_blocking();
            vt100_console_update_draw_screen(fb);
        }
        if(c == 0x7f || c == 0x8){
            if(in->length){
                karray_pop(in, in->length-1);
                kprintf("%c", c);
                kprintf(" ");
                kprintf("%c", c);
            }
            continue;
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
                karray *exp = (karray *)_pythonix_actions_regex[i];
                if(regex_match(exp, line)){
                    karray *matches = regex_match_group(exp, line);
                    if(matches){
                        karray *groups = regex_group_join(matches);
                        if(m){
                            KDEBUG("error line matched more than once", 0x0);
                            for(int j = 0; j < groups->length; j++)
                                KDEBUG("groups[%d] == %s", j, ((char **)groups->array)[j]);
                        }
                        ((void(*)(karray *a, pythonix_vm *vm))_pythonix_actions_pointers[i])(groups, vm);
                        karray_free(groups);
                        karray_free(matches);
                        m = true;
                    }
                    else{
                        KERROR("regex_match(exp, s) == true && regex_match_groups(exp, s) == NULL");
                    }
                }
            }
            if(!m){
                kprintf("error invalid syntax : %s", line);
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
        karray_free((karray *)_pythonix_actions_regex[i]);
}
