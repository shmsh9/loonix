#include <pythonix/pythonix.h>
void pythonix_print_var(karray *a, pythonix_vm *vm){
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
pythonix_type *pythonix_not_impl(karray *a, pythonix_vm *vm){
    kprintf("NotImplError: Function not implemented\n");
    return 0x0;
}
pythonix_type *pythonix_obj_identify_str(karray *groups, pythonix_vm *vm){
    return pythonix_type_str_new(((char **)groups->array)[1], PYTHONIX_VAR_NAME_ANON, vm);
}
pythonix_type *pythonix_obj_identify_int(karray *groups, pythonix_vm *vm){
    return pythonix_type_int_new(atoi(((char **)groups->array)[1]), PYTHONIX_VAR_NAME_ANON, vm);
}
pythonix_type *pythonix_obj_identify_var(karray *groups, pythonix_vm *vm){
    pythonix_type *t = pythonix_vm_get_type(vm, ((char **)groups->array)[1]);
    if(!t){
        kprintf("NameError: name '%s' is not defined\n", ((char **)groups->array)[1]);
        return 0x0;
    }
    return t;
}
pythonix_type *pythonix_obj_identify_method(karray *groups, pythonix_vm *vm){
    pythonix_type *t = pythonix_vm_get_type(vm, ((char **)groups->array)[1]);
    if(!t){
        kprintf("NameError: name '%s' is not defined\n", ((char **)groups->array)[1]);
        return 0x0;
    }
    pythonix_type *t2 = pythonix_type_method_call(t, ((char **)groups->array)[2], 0x0);
    return t2;
}

uint64_t _pythonix_obj_identify_regex[][3] = {
    {0, (uint64_t)"\\s*[',\"](.*)[',\"]\\s*",              (uint64_t)pythonix_obj_identify_str},
    {0, (uint64_t)"\\s*("PYTHONIX_REGEX_INT")\\s*",        (uint64_t)pythonix_obj_identify_int},
    {0, (uint64_t)"\\s*("PYTHONIX_REGEX_VAR_NAME")\\s*",   (uint64_t)pythonix_obj_identify_var},
    {0, (uint64_t)"\\s*("PYTHONIX_REGEX_VAR_NAME")\\s*.\\s*("PYTHONIX_REGEX_METHOD_NAME")\\s*\\((.*)\\)\\s*",(uint64_t)pythonix_obj_identify_method},
    {0, (uint64_t)".*",(uint64_t)pythonix_not_impl}
};
pythonix_type *pythonix_obj_identify(char *val, pythonix_vm *vm){
    for(int i = 0; i < sizeof(_pythonix_obj_identify_regex)/sizeof(_pythonix_obj_identify_regex[0]); i++){
        if(!_pythonix_obj_identify_regex[i][0])
            _pythonix_obj_identify_regex[i][0] = (uint64_t)regex_new((char *)_pythonix_obj_identify_regex[i][1]);
        karray *exp = (karray *)_pythonix_obj_identify_regex[i][0];
        if(regex_match(exp, val)){
            karray *matches = regex_match_group(exp, val);
            if(matches){
                karray *groups = regex_group_join(matches);
                pythonix_type *t = ((pythonix_type *(*)(karray *a, pythonix_vm *vm))_pythonix_obj_identify_regex[i][2])(groups, vm);
                karray_free(groups);
                karray_free(matches);
                return t;
            }
            else{
                KERROR("regex_match(exp, s) == true && regex_match_groups(exp, s) == NULL");
            }
        }
    }
    return 0x0;
}
void pythonix_op_find(uint64_t regex[][3], uint32_t l, char *line, pythonix_vm *vm){
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
                ((void(*)(karray *a, pythonix_vm *vm))regex[i][2])(groups, vm);
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

void pythonix_assign_any(karray *a, pythonix_vm *vm){
    //no need to check for the validity if group(1) when assigning
    //just create it if it is null
    pythonix_type *t2 = pythonix_obj_identify(((char **)a->array)[2], vm);
    if(t2)
        pythonix_type_method_call(t2, "__copy__", ((char **)a->array)[1]);
}
void pythonix_var_operator_x(karray *a, char *method, pythonix_vm *vm){
    pythonix_type *t = pythonix_obj_identify(((char **)a->array)[1], vm);
    if(!t){
        kprintf("NameError: name '%s' is not defined\n", ((char **)a->array)[1]);        
        return;
    }
    pythonix_type *t2 = pythonix_obj_identify(((char **)a->array)[3], vm);
    if(!t2){
        kprintf("NameError: name '%s' is not defined\n", ((char **)a->array)[3]);        
        return;
    }
    pythonix_type_method_call(t, method, (void *)t2);
}

void pythonix_var_operator_any(karray *a, pythonix_vm *vm){
    char *op[] = {
        ['+']="__add__",
        ['-']="__sub__",
        ['*']="__mul__",
        ['/']="__div__",
    };
    pythonix_var_operator_x(a,op[(int)((char**)a->array)[2][0]],vm);
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
uint64_t _pythonix_actions_regex[][3] = {
    {0x0, (uint64_t)PYTHONIX_REGEX_ASSIGN_ANY, (uint64_t)pythonix_assign_any},
    {0x0, (uint64_t)PYTHONIX_REGEX_PRINT_VAR, (uint64_t)pythonix_print_var},
    {0x0, (uint64_t)PYTHONIX_REGEX_FN_CALL, (uint64_t)pythonix_not_impl},
    {0x0, (uint64_t)PYTHONIX_REGEX_FN_DEF, (uint64_t)pythonix_not_impl},
    {0x0, (uint64_t)PYTHONIX_REGEX_METHOD_CALL, (uint64_t)pythonix_method_call},
    {0x0, (uint64_t)PYTHONIX_REGEX_VAR_OPERATOR_ANY, (uint64_t)pythonix_var_operator_any}

};
void pythonix_interpreter(){
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
            pythonix_op_find(
                _pythonix_actions_regex, 
                sizeof(_pythonix_actions_regex)/sizeof(_pythonix_actions_regex[0]), 
                line, 
                vm
            );
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
