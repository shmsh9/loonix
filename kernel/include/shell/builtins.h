#ifndef SHELL_BUILTINS_H_
#define SHELL_BUILTINS_H_
#include <kstd.h>
#define BUILTIN_SIZE 2
struct fnbuiltin {
    int  (*ptrfn)(int, char **);
    char *name;
};

int clear(int argc, char **argv);
int help(int argc, char **argv);


#endif
