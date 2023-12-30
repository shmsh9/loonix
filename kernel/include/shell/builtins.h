#ifndef SHELL_BUILTINS_H_
#define SHELL_BUILTINS_H_

#include <stdint.h>
#include <drivers/pci.h>
#include <shell/gol.h>
#include <vm/vm.h>


struct fnbuiltin {
    int  (*ptrfn)(int, char **);
    char *name;
};

int builtins_clear(int argc, char **argv);
int builtins_help(int argc, char **argv);
int builtins_free(int argc, char **argv);

extern struct fnbuiltin _shell_builtins[];
extern uint32_t _shell_builtins_size;

#define BUILTIN_SIZE sizeof(_shell_builtins)/sizeof(_shell_builtins[0])

#include <drivers/ahci.h>
#include <shell/shell.h>
#include <sys/task.h>
#include <kstd/kstd.h>

#endif
