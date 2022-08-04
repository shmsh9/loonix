#include <shell/builtins.h>

struct fnbuiltins builtins = {0};

int clear(int argc, char **argv){
    kprint("\033[2J\033[H");
    return 0;
}
int help(int argc, char **argv){
    for(int i = 0 ; i < builtins.length; i++){
        kprint(builtins.builtins[i].name);
        kputc('\n');
    }
    return 0;
}
int useless(int argc, char **argv){
    return 0;
}
