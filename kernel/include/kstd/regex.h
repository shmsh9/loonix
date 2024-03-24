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
    karray *alphabet;
} regex_automaton;
#include <kstd/regex_static.h>
void regex_automaton_free(regex_automaton *a);
void regex_automaton_debug_print(regex_automaton *r);
karray *regex_new(char *s);
bool regex_match(karray *at, char *s);
#endif 

