#ifndef _PYTHONIX_TYPES_H_
#define _PYTHONIX_TYPES_H_

#define PYTHONIX_TYPE_NAME_INT "int"
#define PYTHONIX_TYPE_NAME_STR "str"
#define PYTHONIX_VAR_NAME_ANON  0x0

#include <kstd/karray.h>
#include <kstd/khashmap.h>
typedef struct _pythonix_type{
    void *_data;
    karray *_methods;
    khashmap *_methods_hashmap;
    char *name;
    char *_variable_name;
    uint32_t _ref_count;
    void (*_free)(struct _pythonix_type *);
    struct _pythonix_type *(*_copy)(struct _pythonix_type *, void*);
    struct _pythonix_type *(*_str)(struct _pythonix_type *, void*);
    void *_vm;
}pythonix_type;

typedef struct _pythonix_type_str{
    char *data;
    uint64_t length;
}pythonix_type_str;

typedef struct _pythonix_method{
    pythonix_type *(*_method)(pythonix_type *, void *);
    char *name;
}pythonix_method;
#include <pythonix/pythonix.h>

void pythonix_method_free(pythonix_method *m);
void pythonix_type_free(pythonix_type *t);
pythonix_method *pythonix_method_new(char *name, pythonix_type* (*m)(pythonix_type*,void*));
pythonix_type *pythonix_type_new(char *name, char *variable_name, pythonix_vm *vm);
pythonix_type *pythonix_type_method_call(pythonix_type *self, char *m, void *p);
pythonix_method *pythonix_type_method_get(pythonix_type *t, char *m);

void pythonix_type_method_add(pythonix_type *t, pythonix_method *m);

pythonix_type *pythonix_type_int_new(int64_t value, char *vname, pythonix_vm *vm);
pythonix_type *pythonix_type_str_new(char *value, char *vname, pythonix_vm *vm);
#endif
