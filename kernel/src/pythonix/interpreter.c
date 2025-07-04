#include <pythonix/pythonix.h>
pythonix_type *pythonix_not_impl(karray *a, pythonix_vm *vm){
    kprintf("NotImplError: Function not implemented\n");
    return 0x0;
}
pythonix_type *pythonix_obj_identify_str(karray *groups, pythonix_vm *vm){
    if(groups->length < 1){
        KERROR("groups->length < 1");
        return 0x0;
    }
    return pythonix_type_str_new(((char **)groups->array)[1], PYTHONIX_VAR_NAME_ANON, vm);
}
pythonix_type *pythonix_obj_identify_int(karray *groups, pythonix_vm *vm){
    if(groups->length < 1){
        KERROR("groups->length < 1");
        return 0x0;
    }
    return pythonix_type_int_new(atoi(((char **)groups->array)[1]), PYTHONIX_VAR_NAME_ANON, vm);
}
pythonix_type *pythonix_obj_identify_var(karray *groups, pythonix_vm *vm){
    if(groups->length < 1){
        KERROR("groups->length < 1");
        return 0x0;
    }

    pythonix_type *t = pythonix_vm_get_type(vm, ((char **)groups->array)[1]);
    if(!t){
        kprintf("NameError: name '%s' is not defined\n", ((char **)groups->array)[1]);
        return 0x0;
    }
    return t;
}
pythonix_type *pythonix_obj_identify_method(karray *groups, pythonix_vm *vm){
    if(groups->length < 2){
        KERROR("groups->length < 2");
        return 0x0;
    }
    pythonix_type *t = pythonix_vm_get_type(vm, ((char **)groups->array)[1]);
    if(!t){
        kprintf("NameError: name '%s' is not defined\n", ((char **)groups->array)[1]);
        return 0x0;
    }
    pythonix_type *t2 = pythonix_type_method_call(t, ((char **)groups->array)[2], 0x0);
    return t2;
}

pythonix_type *pythonix_obj_identify(char *val, pythonix_vm *vm){
    static uint64_t _pythonix_obj_identify_regex[][3] = {
        {0, (uint64_t)"\\s*[',\"](.*)[',\"]\\s*",              (uint64_t)pythonix_obj_identify_str},
        {0, (uint64_t)"\\s*("PYTHONIX_REGEX_INT")\\s*",        (uint64_t)pythonix_obj_identify_int},
        {0, (uint64_t)"\\s*("PYTHONIX_REGEX_VAR_NAME")\\s*",   (uint64_t)pythonix_obj_identify_var},
        {0, (uint64_t)"\\s*("PYTHONIX_REGEX_VAR_NAME")\\s*.\\s*("PYTHONIX_REGEX_METHOD_NAME")\\s*\\((.*)\\)\\s*",(uint64_t)pythonix_obj_identify_method},
        {0, (uint64_t)".*",(uint64_t)pythonix_not_impl}
    };
    int regx_len = sizeof(_pythonix_obj_identify_regex)/sizeof(_pythonix_obj_identify_regex[0]);

    for(int j = 0; j < regx_len; j ++)
        _pythonix_obj_identify_regex[j][0] = (uint64_t)regex_new((char *)_pythonix_obj_identify_regex[j][1]);;

    for(int i = 0; i < regx_len; i++){

        karray *exp = (karray *)_pythonix_obj_identify_regex[i][0];
        if(regex_match(exp, val)){
            karray *matches = regex_match_group(exp, val);
            if(matches && matches->length){
                karray *groups = regex_group_join(matches);
                pythonix_type *t = ((pythonix_type *(*)(karray *a, pythonix_vm *vm))_pythonix_obj_identify_regex[i][2])(groups, vm);
                //please fixxx
                karray_free(groups);
                karray_free(matches);
                for(int j = 0; j < regx_len; j ++)
                    karray_free((karray *)_pythonix_obj_identify_regex[j][0]);
                return t;
            }
            else{
                KERROR("regex_match(exp, s) == true && regex_match_groups(exp, s) == NULL");
                KDEBUG("%s && %s", _pythonix_obj_identify_regex[i][1], val);
                for(int j = 0; j < regx_len; j ++)
                    karray_free((karray *)_pythonix_obj_identify_regex[j][0]);
                return 0x0;
            }

        }
    }
    for(int j = 0; j < regx_len; j ++)
        karray_free((karray *)_pythonix_obj_identify_regex[j][0]);
    return 0x0;
}
void pythonix_print_var(karray *a, pythonix_vm *vm){
    if(a->length < 1){
        KERROR("a->length < 1");
        return;
    }
    char *name = ((char **)a->array)[1];
    pythonix_type *t = pythonix_obj_identify(name, vm);
    if(!t)
        return;
    pythonix_type *str = pythonix_type_method_call(t, "__str__", (void *)vm);
    if(str && !strcmp(PYTHONIX_TYPE_NAME_STR, str->name))
        kprintf("%s\n", ((pythonix_type_str *)(str->_data))->data);
}

void pythonix_assign_any(karray *a, pythonix_vm *vm){
    //no need to check for the validity if group(1) when assigning
    //just create it if it is null
    if(a->length < 2){
        KERROR("a->length < 2");
        return;
    }
    pythonix_type *t = pythonix_obj_identify(((char **)a->array)[2], vm);
    if(t){
        KDEBUG("t.__copy__ (0x%x) on %s", t, ((char **)a->array)[1]);
        pythonix_type_method_call(t, "__copy__", ((char **)a->array)[1]);
    }
}
void pythonix_var_operator_x(karray *a, char *method, pythonix_vm *vm){
    pythonix_type *t = pythonix_obj_identify(((char **)a->array)[1], vm);
    if(!t){
        return;
    }
    pythonix_type *t2 = pythonix_obj_identify(((char **)a->array)[3], vm);
    if(!t2){
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
    if(a->length < 2){
        KERROR("a->length < 2");
        return;
    }
    char *name =  ((char **)a->array)[PYTHONIX_REGEX_METHOD_CALL_VAR_NAME_GROUP];
    char *method =  ((char **)a->array)[PYTHONIX_REGEX_METHOD_CALL_METHOD_NAME_GROUP];
    pythonix_type *t = pythonix_obj_identify(name, vm);
    if(!t){
        return;
    }
    pythonix_type *t2 = (pythonix_type *)pythonix_type_method_call(t, method, 0x0);
    if(!strcmp(t->name, PYTHONIX_TYPE_NAME_STR) && !strcmp(method, "__str__")){
        kprintf("%s\n", ((pythonix_type_str *)(t2->_data))->data);
        return;
    }
    if(t2){
        pythonix_method *has_str = pythonix_type_method_get(t2, "__str__");
        if(has_str){
            pythonix_type *str = (pythonix_type *)pythonix_type_method_call(t2, "__str__", 0x0);
            kprintf("%s\n", ((pythonix_type_str *)(str->_data))->data);
        }
    }
}
pythonix_type *pythonix_global_help__str__(pythonix_type *t, void *vm){
    return pythonix_type_str_new(
        "Type help() for interactive help, or help(object) for help about object.", 
        PYTHONIX_VAR_NAME_ANON, 
        (pythonix_vm *)vm
    );
}
bool pythonix_interpreter_quitted = false;
pythonix_type *pythonix_interpreter_quit(pythonix_type *t, void *vm){
    pythonix_interpreter_quitted = true;
    return pythonix_type_str_new(
        "Use quit() or Ctrl-Z plus Return to exit", 
        PYTHONIX_VAR_NAME_ANON, 
        (pythonix_vm *)vm
    );
}
void pythonix_interpreter_init(pythonix_vm *vm){
    pythonix_type *help = pythonix_type_new("global", "help", vm);
    pythonix_type *quit = pythonix_type_new("global", "quit", vm);
    pythonix_type_method_add(quit, pythonix_method_new("__str__", pythonix_interpreter_quit));
    pythonix_type_method_add(help, pythonix_method_new("__str__", pythonix_global_help__str__));
}
void pythonix_interpreter(){
    static uint64_t _pythonix_actions_regex[][3] = {
        {0x0, (uint64_t)PYTHONIX_REGEX_ASSIGN_ANY, (uint64_t)pythonix_assign_any},
        {0x0, (uint64_t)PYTHONIX_REGEX_PRINT_VAR, (uint64_t)pythonix_print_var},
        {0x0, (uint64_t)PYTHONIX_REGEX_FN_CALL, (uint64_t)pythonix_not_impl},
        {0x0, (uint64_t)PYTHONIX_REGEX_FN_DEF, (uint64_t)pythonix_not_impl},
        {0x0, (uint64_t)PYTHONIX_REGEX_METHOD_CALL, (uint64_t)pythonix_method_call},
        {0x0, (uint64_t)PYTHONIX_REGEX_VAR_OPERATOR_ANY, (uint64_t)pythonix_var_operator_any}
    };
    int regx_len = sizeof(_pythonix_actions_regex)/sizeof(_pythonix_actions_regex[0]);
    for(int i = 0; i < regx_len; i++)
        _pythonix_actions_regex[i][0] = (uint64_t)regex_new((char *)_pythonix_actions_regex[i][1]);

    kprint(
        "Pythonix 0.0.1 on loonix\n"
        "Type \"help\", \"copyright\", \"credits\" or \"license\" for more information.\n"
    );
    pythonix_vm *vm = pythonix_vm_new();
    pythonix_interpreter_init(vm);
    karray *in = karray_new(sizeof(char), NULL);
    kprintf(">>> ");
    while(true && !pythonix_interpreter_quitted){
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
            for(int i = 0; i < in->length; i++)
                line[i] = ((char *)in->array)[i];
            bool m = false;
            for(int i = 0; i < regx_len; i++){
                karray *exp = (karray *)_pythonix_actions_regex[i][0];
                if(regex_match(exp, line)){
                    karray *matches = regex_match_group(exp, line);
                    if(matches && matches->length){
                        if(m){
                            KDEBUG("matched more than once regex[%d]", i);
                        }
                        m = true;
                        karray *groups = regex_group_join(matches);
                        ((void(*)(karray *a, pythonix_vm *vm))_pythonix_actions_regex[i][2])(groups, vm);
                        karray_free(groups);
                        karray_free(matches);
                    }
                    else{
                        KERROR("regex_match(exp, s) == true && regex_match_groups(exp, s) == NULL");
                        KDEBUG("%s && %s", _pythonix_actions_regex[i][1], line ? line : "NULL");

                    }
                }
            }
            if(!m)
                kprintf("error invalid syntax : %s", line);

            kfree(line);
            karray_clear(in);
            pythonix_vm_gc(vm);
            kprintf(">>> ");
        }
    }
    pythonix_interpreter_quitted = false;
    pythonix_vm_free(vm);
    karray_free(in);

    for(int i = 0; i < regx_len; i++)
        karray_free((karray *)_pythonix_actions_regex[i][0]);
    
    kprint("\n");
}
