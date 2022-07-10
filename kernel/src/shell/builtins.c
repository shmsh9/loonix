#include <shell/builtins.h>

struct fnbuiltin builtins[BUILTIN_SIZE];

int clear(int argc, char **argv){
    kprint("\033[2J\033[H");
    return 0;
}
int help(int argc, char **argv){
    for(int i = 0 ; i < BUILTIN_SIZE; i++){
        kprint(builtins[i].name);
        kputc('\n');
    }
    return 0;
}

