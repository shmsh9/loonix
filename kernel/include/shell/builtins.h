#ifndef SHELL_BUILTINS_H_
#define SHELL_BUILTINS_H_

#include <stdint.h>
#include <drivers/pci.h>
#include <shell/gol.h>


#define _BUILTIN_NAME 0
#define _BUILTIN_FN 1
#define _BUILTINS_SIZE (sizeof(_shell_builtins)/sizeof(_shell_builtins[0]))

extern uint64_t _shell_builtins[][2];
extern int _builtins_size;

int builtins_clear(int argc, char **argv);
int builtins_help(int argc, char **argv);
int builtins_free(int argc, char **argv);
bool bulitins_is_builtin(char *cmd);

#include <drivers/ahci.h>
#include <shell/shell.h>
#include <sys/task.h>
#include <kstd/kstd.h>

#endif
