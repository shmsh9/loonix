#ifndef _REGEX_STATIC_H
#define _REGEX_STATIC_H

#define _REGEX_STATIC_KARRAY_a ((char[]){ \
    'a','b','c','d','e','f','g','h','i','j','k','l'\
})
#define _regex_automaton_static(a,l) ({\
    &(regex_automaton){\
        .length = l,\
        .alphabet = a\
    };\
})
#define _regex_static(expr) ({ \
    regex_automaton *_automaton_static_a = _regex_automaton_static(_karray_static(((char[]){'a'})), 1);\
    regex_automaton *_automaton_static_b = _regex_automaton_static(_karray_static(((char[]){'b'})), 1);\
    regex_automaton *_automaton_static_c = _regex_automaton_static(_karray_static(((char[]){'c'})), 1);\
    (_karray_static( \
        ((uint64_t[]){ \
            (uint64_t)_automaton_static_a,\
            (uint64_t)_automaton_static_b, \
            (uint64_t)_automaton_static_c \
        }) \
    )); \
})

#endif
