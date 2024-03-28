#include <pythonix/pythonix.h>
void pythonix_assign_int(karray *a, pythonix_vm *vm){
    //KDEBUG("called with a == 0x%x", a);
    khashmap_set(
        vm->names,
        ((char **)a->array)[PYTHONIX_REGEX_ASSIGN_INT_NAME_GROUP],
        atoi(((char **)a->array)[PYTHONIX_REGEX_ASSIGN_INT_VAL_GROUP])
    );
}
void pythonix_assign_var(karray *a, pythonix_vm *vm){
    //KDEBUG("called with a == 0x%x", a);
}
void pythonix_assign_str(karray *a, pythonix_vm *vm){
    //KDEBUG("called with a == 0x%x", a);
}
void pythonix_print_var(karray *a, pythonix_vm *vm){
    //KDEBUG("called with a == 0x%x", a);
    kprintf("%d\n", 
        khashmap_get(
            vm->names, 
            ((char **)a->array)[PYTHONIX_REGEX_PRINT_VAR_NAME_GROUP]
        )
    );
}
void pythonix_not_impl(karray *a, pythonix_vm *vm){
    KDEBUG("called with a == 0x%x", a);
}
void pythonix_interpreter(){
    uint64_t _pythonix_actions_regex[][2] = {
        [PYTHONIX_ASSIGN_STR]={(uint64_t)"PYTHONIX_REGEX_ASSIGN_STR", (uint64_t)regex_new(PYTHONIX_REGEX_ASSIGN_STR)},
        [PYTHONIX_ASSIGN_INT]={(uint64_t)"PYTHONIX_REGEX_ASSIGN_INT", (uint64_t)regex_new(PYTHONIX_REGEX_ASSIGN_INT)},
        [PYTHONIX_ASSIGN_VAR]={(uint64_t)"PYTHONIX_REGEX_ASSIGN_VAR", (uint64_t)regex_new(PYTHONIX_REGEX_ASSIGN_VAR)},
        [PYTHONIX_PRINT_VAR ]={(uint64_t)"PYTHONIX_REGEX_PRINT_VAR", (uint64_t)regex_new(PYTHONIX_REGEX_PRINT_VAR)},
        [PYTHONIX_FN_CALL   ]={(uint64_t)"PYTHONIX_REGEX_FN_CALL", (uint64_t)regex_new(PYTHONIX_REGEX_FN_CALL)},
        [PYTHONIX_FN_DEF    ]={(uint64_t)"PYTHONIX_REGEX_FN_DEF", (uint64_t)regex_new(PYTHONIX_REGEX_FN_DEF)}
    };
    uint64_t _pythonix_actions_pointers[sizeof(_pythonix_actions_regex)/sizeof(_pythonix_actions_regex[0])] = {
        [PYTHONIX_ASSIGN_STR]=(uint64_t)pythonix_assign_str,
        [PYTHONIX_ASSIGN_INT]=(uint64_t)pythonix_assign_int,
        [PYTHONIX_ASSIGN_VAR]=(uint64_t)pythonix_assign_var,
        [PYTHONIX_PRINT_VAR ]=(uint64_t)pythonix_print_var,
        [PYTHONIX_FN_CALL   ]=(uint64_t)pythonix_not_impl,
        [PYTHONIX_FN_DEF    ]=(uint64_t)pythonix_not_impl
    };

    pythonix_vm *vm = pythonix_vm_new();
    karray *in = karray_new(sizeof(char), NULL);
    kprintf(">>> ");
    while(true){
        char c = kgetchar_non_blocking();
        while(!c)
            c = kgetchar_non_blocking();
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
            kprintf(">>> ");
        }
    }
    pythonix_vm_free(vm);
}
