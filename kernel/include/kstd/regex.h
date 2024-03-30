#ifndef REGEX_H_
#define REGEX_H_
#include <kstd/kstd.h>
#include <kstd/cmp.h>

#define REGEX_INF_ZR_LEN UINT32_MAX 
#define REGEX_OPT_LEN 0
#define REGEX_SPECIAL_CHARS '{','}','*','?','[', ']','(',')','.','\\','-', ',','$','^'
#define REGEX_LENGTH_CHARS  '{','}','*','?'
#define REGEX_WS_CHARS '\r','\t','\n',' '
#define REGEX_CHAR_RANGE_STOP(a,b) MAX(a,b)
#define REGEX_CHAR_RANGE_START(a,b) MIN(a,b)
typedef struct _regex_automaton{
    uint32_t length;
    uint32_t group;
    karray *alphabet;
} regex_automaton;
typedef struct _regex_match_string{
    uint32_t group;
    char *string;
    uint32_t length;
}regex_match_string;
#include <kstd/regex_static.h>
void regex_automaton_free(regex_automaton *a);
void regex_automaton_debug_print(regex_automaton *r);
karray *regex_new(char *s);
bool regex_match(karray *at, char *s);
karray * regex_group_join(karray *g);
karray * regex_match_group(karray *at, char *s);
void regex_match_free(regex_match_string *m);
#endif 

