#include <pythonix/pythonix.h>
/*
void pythonix_assign_str(karray *a, pythonix_vm *vm){
void pythonix_parse(char *s){
    bool m = false;
    
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
}
*/
