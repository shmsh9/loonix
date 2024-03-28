#include <pythonix/pythonix.h>

void pythonix_assign_str(karray *a, pythonix_vm *vm){
    KDEBUG("called with a == 0x%x", a);
}
void pythonix_assign_int(karray *a, pythonix_vm *vm){
    KDEBUG("called with a == 0x%x", a);
    khashmap_set(
        vm->names,
        ((char **)a->array)[PYTHONIX_REGEX_ASSIGN_INT_NAME_GROUP],
        atoi(((char **)a->array)[PYTHONIX_REGEX_ASSIGN_INT_VAL_GROUP])
    );
}
void pythonix_assign_var(karray *a, pythonix_vm *vm){
    KDEBUG("called with a == 0x%x", a);
}
void pythonix_print_var(karray *a, pythonix_vm *vm){
    KDEBUG("called with a == 0x%x", a);
}
void pythonix_not_impl(karray *a, pythonix_vm *vm){
    KDEBUG("called with a == 0x%x", a);
}
void pythonix_parse(char *s){
    uint64_t possible[][2] = {
        [PYTHONIX_ASSIGN_STR]={(uint64_t)"PYTHONIX_REGEX_ASSIGN_STR", (uint64_t)regex_new(PYTHONIX_REGEX_ASSIGN_STR)},
        [PYTHONIX_ASSIGN_INT]={(uint64_t)"PYTHONIX_REGEX_ASSIGN_INT", (uint64_t)regex_new(PYTHONIX_REGEX_ASSIGN_INT)},
        [PYTHONIX_ASSIGN_VAR]={(uint64_t)"PYTHONIX_REGEX_ASSIGN_VAR", (uint64_t)regex_new(PYTHONIX_REGEX_ASSIGN_VAR)},
        [PYTHONIX_PRINT_VAR ]={(uint64_t)"PYTHONIX_REGEX_PRINT_VAR", (uint64_t)regex_new(PYTHONIX_REGEX_PRINT_VAR)},
        [PYTHONIX_FN_CALL   ]={(uint64_t)"PYTHONIX_REGEX_FN_CALL", (uint64_t)regex_new(PYTHONIX_REGEX_FN_CALL)},
        [PYTHONIX_FN_DEF    ]={(uint64_t)"PYTHONIX_REGEX_FN_DEF", (uint64_t)regex_new(PYTHONIX_REGEX_FN_DEF)}
    };
    uint64_t actions[] = {
        [PYTHONIX_ASSIGN_STR]=(uint64_t)pythonix_assign_str,
        [PYTHONIX_ASSIGN_INT]=(uint64_t)pythonix_assign_int,
        [PYTHONIX_ASSIGN_VAR]=(uint64_t)pythonix_assign_var,
        [PYTHONIX_PRINT_VAR ]=(uint64_t)pythonix_print_var,
        [PYTHONIX_FN_CALL   ]=(uint64_t)pythonix_not_impl,
        [PYTHONIX_FN_DEF    ]=(uint64_t)pythonix_not_impl
    };
    bool m = false;
    pythonix_vm *vm = pythonix_vm_new();
    for(int i = 0; i < sizeof(possible)/sizeof(possible[0]); i++){
        karray *exp = (karray *)possible[i][1];
        //char *exp_name =  (char *)possible[i][0];
        if(regex_match(exp, s)){
            m = true;
            karray *matches = regex_match_group(exp, s);
            if(matches){
                karray *groups = regex_group_join(matches);
                //KDEBUG("%s is %s", s, exp_name);
                //for(int i = 0; i < groups->length; i++){
                //    KDEBUG("groups[%d] == %s", i, ((char **)groups->array)[i]);
                //}
                ((void(*)(karray *a, pythonix_vm *vm))actions[i])(groups, vm);
                karray_free(groups);
                karray_free(matches);
            }
            else{
                KERROR("regex_match(exp, s) == true && regex_match_groups(exp, s) == NULL");
            }
        }
    }
    if(!m){
        KDEBUG("invalid syntax %s", s);
    }
    khashmap_debug_print(vm->names);
    pythonix_vm_free(vm);
}
