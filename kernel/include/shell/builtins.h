#ifndef SHELL_BUILTINS_H_
#define SHELL_BUILTINS_H_
#include <kstd.h>
#define BUILTIN_SIZE 0xFF
#define SHELL_INIT_BUILTIN(fn, n){\
    if(builtins.length < BUILTIN_SIZE){\
        builtins.builtins[builtins.length].name  = n;\
        builtins.builtins[builtins.length].ptrfn = fn;\
        builtins.length++;\
    }\
}

struct fnbuiltin {
    int  (*ptrfn)(int, char **);
    char *name;
};
struct fnbuiltins {
    struct fnbuiltin builtins[BUILTIN_SIZE];
    uint8_t length;
};

int clear(int argc, char **argv);
int help(int argc, char **argv);
int useless(int argc, char **argv);

extern struct fnbuiltins builtins;

#endif
