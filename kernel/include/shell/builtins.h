#ifndef SHELL_BUILTINS_H_
#define SHELL_BUILTINS_H_
#include <kstd.h>
#include <drivers/ahci.h>
#include <drivers/pci.h>

#define BUILTIN_SIZE 16
#define BUILTINS_INIT_FN(fn, n){\
    if(builtins.length < BUILTIN_SIZE){\
        builtins.builtins[builtins.length].name  = n;\
        builtins.builtins[builtins.length].ptrfn = fn;\
        builtins.length++;\
    }\
    else{\
        KERROR("builtins.length >= BUILTIN_SIZE");\
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

int builtins_clear(int argc, char **argv);
int builtins_help(int argc, char **argv);
int builtins_free(int argc, char **argv);
void builtins_init();
extern struct fnbuiltins builtins;

#endif
