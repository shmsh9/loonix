#ifndef REGEX_H_
#define REGEX_H_
#include <kstd/kstd.h>
#include <kstd/cmp.h>
#include <kstd/regex_static.h>

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
#endif 

