#include <shell/builtins.h>

const struct fnbuiltin builtins[BUILTIN_SIZE] = {
        { clear, "clear"},
        { help,  "help"}
};
int clear(int argc, char **argv){
    kprint("\033[2J\033[H");
    return 0;
}
int help(int argc, char **argv){
    for(int i = 0 ; i < BUILTIN_SIZE; i++){
        kprintf("%s\n", builtins[i].name);    
    }
    return 0;
}

