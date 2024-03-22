#ifndef REGEX_H_
#define REGEX_H_
#include <kstd/kstd.h>
#include <kstd/cmp.h>

#define REGEX_INF_ZR_LEN UINT32_MAX 
#define REGEX_OPT_LEN 0
typedef struct _regex_automaton{
    uint32_t length;
    karray *alphabet;
} regex_automaton;
void regex_automaton_free(regex_automaton *a);
void regex_automaton_debug_print(regex_automaton *r);
karray *regex_new(char *s);
bool regex_match(karray *at, char *s);
#define _regex_automaton_static(a,l) ({\
    &(regex_automaton){\
        .length = l,\
        .alphabet = a\
    };\
})
#define _regex_static(expr) ({ \
    \
})
#endif 

