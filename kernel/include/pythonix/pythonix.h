#ifndef _PYTHONIX_H_
#define _PYTHONIX_H_
#include <kstd/regex.h>
#include <kstd/khashmap.h>
#include <pythonix/grammar.h>

#define PYTHONIX_ASSIGN_STR 0
#define PYTHONIX_ASSIGN_INT 1
#define PYTHONIX_ASSIGN_VAR 2
#define PYTHONIX_PRINT_VAR  3
#define PYTHONIX_FN_CALL    4
#define PYTHONIX_FN_DEF     5

typedef struct _pythonix_vm{
    khashmap *names;
}pythonix_vm;

void pythonix_parse(char *s);
pythonix_vm *pythonix_vm_new();
void pythonix_vm_free(pythonix_vm *vm);
#endif
