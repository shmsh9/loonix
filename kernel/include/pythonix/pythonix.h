#ifndef _PYTHONIX_H_
#define _PYTHONIX_H_
#include <kstd/regex.h>
#include <kstd/khashmap.h>
#include <pythonix/grammar.h>

#define PYTHONIX_ASSIGN_STR             0
#define PYTHONIX_ASSIGN_INT             1
#define PYTHONIX_ASSIGN_VAR             2

#define PYTHONIX_VAR_OPERATOR_ADD       0
#define PYTHONIX_VAR_OPERATOR_SUB       1
#define PYTHONIX_VAR_OPERATOR_MUL       2
#define PYTHONIX_VAR_OPERATOR_DIV       3

#define PYTHONIX_VAR_OPERATOR_ADD_STR   0
#define PYTHONIX_VAR_OPERATOR_ADD_INT   1
#define PYTHONIX_VAR_OPERATOR_ADD_VAR   3

#define PYTHONIX_ASSIGN_ANY             0
#define PYTHONIX_PRINT_VAR              1
#define PYTHONIX_FN_CALL                2
#define PYTHONIX_FN_DEF                 3
#define PYTHONIX_METHOD_CALL            4
#define PYTHONIX_VAR_OPERATOR_ANY       5

typedef struct _pythonix_vm{
    khashmap *names;
    karray *types;
}pythonix_vm;
#include <pythonix/types.h>
pythonix_vm *pythonix_vm_new();
pythonix_type *pythonix_vm_get_type(pythonix_vm *vm, char *name);
void pythonix_vm_free(pythonix_vm *vm);
void pythonix_interpreter();
void pythonix_vm_gc(pythonix_vm *vm);
#endif
