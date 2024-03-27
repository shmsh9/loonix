#ifndef _REGEX_STATIC_H
#define _REGEX_STATIC_H
#include <kstd/regex.h>
#include <kstd/kstd.h>
#include <preprocessed/loop_uint.h>

#define _REGEX_STATIC_ANY _LOOP_UINT(255)
#define _REGEX_STATIC_0_9 '0','1','2','3','4','5','6','7','8','9'
#define _REGEX_STATIC_a_z 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
#define _REGEX_STATIC_A_Z 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
#define _REGEX_STATIC_ALPHA _REGEX_STATIC_A_Z,_REGEX_STATIC_a_z
#define _REGEX_STATIC_ALPHANUM _REGEX_STATIC_ALPHA,_REGEX_STATIC_0_9
#define _REGEX_STATIC_WS REGEX_WS_CHARS

#define _regex_static_bracket_comma(expr, i, n) { _regex_static_bracket_comma##n(expr,i) }
#define _regex_static_bracket_comma6(expr, i) expr[i+11], _regex_static_bracket_comma5(expr,i)
#define _regex_static_bracket_comma5(expr, i) expr[i+9], _regex_static_bracket_comma4(expr,i)
#define _regex_static_bracket_comma4(expr, i) expr[i+7], _regex_static_bracket_comma3(expr,i)
#define _regex_static_bracket_comma3(expr, i) expr[i+5], _regex_static_bracket_comma2(expr,i)
#define _regex_static_bracket_comma2(expr, i) expr[i+3], _regex_static_bracket_comma1(expr,i)
#define _regex_static_bracket_comma1(expr, i) expr[i+1]

#define _regex_static_bracket_count9(expr,i,r) ((expr[i+9] != ']' && expr[i+9] != ',') ?  r+1 : _regex_static_bracket_count8(expr,i,r))
#define _regex_static_bracket_count8(expr,i,r) ((expr[i+8] != ']' && expr[i+8] != ',') ?  r+1 : _regex_static_bracket_count7(expr,i,r))
#define _regex_static_bracket_count7(expr,i,r) ((expr[i+7] != ']' && expr[i+7] != ',') ?  r+1 : _regex_static_bracket_count6(expr,i,r))
#define _regex_static_bracket_count6(expr,i,r) ((expr[i+6] != ']' && expr[i+6] != ',') ?  r+1 : _regex_static_bracket_count5(expr,i,r))
#define _regex_static_bracket_count5(expr,i,r) ((expr[i+5] != ']' && expr[i+5] != ',') ?  r+1 : _regex_static_bracket_count4(expr,i,r))
#define _regex_static_bracket_count4(expr,i,r) ((expr[i+4] != ']' && expr[i+4] != ',') ?  r+1 : _regex_static_bracket_count3(expr,i,r))
#define _regex_static_bracket_count3(expr,i,r) ((expr[i+3] != ']' && expr[i+3] != ',') ?  r+1 : _regex_static_bracket_count2(expr,i,r))
#define _regex_static_bracket_count2(expr,i,r) ((expr[i+2] != ']' && expr[i+2] != ',') ?  r+1 : _regex_static_bracket_count1(expr,i,r))
#define _regex_static_bracket_count1(expr,i,r) ((expr[i+1] != ']' && expr[i+1] != ',') ?  r+1 : _regex_static_bracket_count0(expr,i,r))
#define _regex_static_bracket_count0(expr,i,r) ((expr[i] != ']'   && expr[i] != ','  ) ?  r+1 : r)

#define _regex_static_bracket_count(expr, i)({\
    uint32_t r = 0;\
    switch ((sizeof(expr)/sizeof(expr[0]))-1-i){ \
        case 1: \
            r = _regex_static_bracket_count0(expr,i,r);\
            break;\
        case 2:\
            r = _regex_static_bracket_count1(expr,i,r);\
            break;\
        case 3:\
            r = _regex_static_bracket_count2(expr,i,r);\
            break;\
        case 4:\
            r = _regex_static_bracket_count3(expr,i,r);\
            break;\
        case 5:\
            r = _regex_static_bracket_count4(expr,i,r);\
            break;\
        case 6:\
            r = _regex_static_bracket_count5(expr,i,r);\
            break;\
        case 7:\
            r = _regex_static_bracket_count6(expr,i,r);\
            break;\
        case 8:\
            r = _regex_static_bracket_count7(expr,i,r);\
            break;\
        case 9:\
            r = _regex_static_bracket_count8(expr,i,r);\
            break;\
        case 10:\
            r = _regex_static_bracket_count9(expr,i,r);\
            break;\
        default:\
            r = UINT32_MAX;\
            break;\
        }\
    r;\
})

#define _regex_static_bracket_start9(expr,r) (expr[9] == '[' ?  9 : _regex_static_bracket_start8(expr, r))
#define _regex_static_bracket_start8(expr,r) (expr[8] == '[' ?  8 : _regex_static_bracket_start7(expr, r))
#define _regex_static_bracket_start7(expr,r) (expr[7] == '[' ?  7 : _regex_static_bracket_start6(expr, r))
#define _regex_static_bracket_start6(expr,r) (expr[6] == '[' ?  6 : _regex_static_bracket_start5(expr, r))
#define _regex_static_bracket_start5(expr,r) (expr[5] == '[' ?  5 : _regex_static_bracket_start4(expr, r))
#define _regex_static_bracket_start4(expr,r) (expr[4] == '[' ?  4 : _regex_static_bracket_start3(expr, r))
#define _regex_static_bracket_start3(expr,r) (expr[3] == '[' ?  3 : _regex_static_bracket_start2(expr, r))
#define _regex_static_bracket_start2(expr,r) (expr[2] == '[' ?  2 : _regex_static_bracket_start1(expr, r))
#define _regex_static_bracket_start1(expr,r) (expr[1] == '[' ?  1 : _regex_static_bracket_start0(expr, r))
#define _regex_static_bracket_start0(expr,r) (expr[0] == '[' ?  0 : r)

#define _regex_static_bracket_start(expr)({\
    uint32_t r = UINT32_MAX;\
    switch ((sizeof(expr)/sizeof(expr[0]))-1){ \
        case 1: \
            r = _regex_static_bracket_start0(expr,r);\
            break;\
        case 2:\
            r = _regex_static_bracket_start1(expr,r);\
            break;\
        case 3:\
            r = _regex_static_bracket_start2(expr,r);\
            break;\
        case 4:\
            r = _regex_static_bracket_start3(expr,r);\
            break;\
        case 5:\
            r = _regex_static_bracket_start4(expr,r);\
            break;\
        case 6:\
            r = _regex_static_bracket_start5(expr,r);\
            break;\
        case 7:\
            r = _regex_static_bracket_start8(expr,r);\
            break;\
        case 8:\
            r = _regex_static_bracket_start9(expr,r);\
            break;\
        default:\
            break;\
        }\
    r;\
})

#define _regex_dict(...) __VA_ARGS__
#define _regex_automaton_static(a,l) ({\
    (uint64_t)&(regex_automaton){\
        .length = l,\
        .alphabet = _karray_static(((uint8_t[])a))\
    };\
})
#define _regex_static(...) _karray_static( ((uint64_t[]){ __VA_ARGS__ }) )
#define _regex_static_new_set(i,a,n) \
    for(volatile int _regex_static_new_set_i = 0;_regex_static_new_set_i < sizeof(a)/sizeof(a[0]);_regex_static_new_set_i++)\
        _regex_static_new_arr_raw[i][_regex_static_new_set_i] = a[_regex_static_new_set_i];\
    _regex_static_new_arr[i] = (karray){ \
        .length = sizeof(a)/sizeof(a[0]), .alloc = UINT8_MAX, \
        .array = _regex_static_new_arr_raw+i, \
        .elementsz = sizeof(a[0]) \
    }; \
    _regex_static_new_at[i] = (regex_automaton){.length = n, .alphabet = _regex_static_new_arr+i};\
    _regex_static_new_ret[i] = (uint64_t)(_regex_static_new_at+i);\
    ret->length++;\
    i++;\

#define _regex_static_new_push(i,c,n) \
    _regex_static_new_arr_raw[i][n++] = c;\
    _regex_static_new_arr[i] = (karray){ \
        .length = n, .alloc = UINT8_MAX, \
        .array = _regex_static_new_arr_raw+i, \
        .elementsz = sizeof(c) \
    }; \
    _regex_static_new_at[i] = (regex_automaton){.length = 1, .alphabet = _regex_static_new_arr+i};\
    _regex_static_new_ret[i] = (uint64_t)(_regex_static_new_at+i);\

#define _regex_static_new(expr) ({\
    static uint64_t _regex_static_new_ret[sizeof(expr)-1] = {0};\
    static uint8_t _regex_static_new_arr_raw[sizeof(expr)-1][UINT8_MAX] = {0};\
    static karray _regex_static_new_arr[sizeof(expr)-1] = {0}; \
    static regex_automaton _regex_static_new_at[sizeof(expr)-1] = {0};\
    karray *ret = _karray_static(_regex_static_new_ret);\
    ret->length = 0;\
    int _curr_at = 0;\
    for(int _regex_static_new_i = 0; _regex_static_new_i < sizeof(expr)-1; _regex_static_new_i++){\
	    switch(expr[_regex_static_new_i]){\
	        case '.':\
                    _regex_static_new_set(_curr_at,((uint8_t[]){_REGEX_STATIC_ANY}),1);\
	    	break;\
	        case '?':\
	            _regex_static_new_at[_curr_at-1].length = REGEX_OPT_LEN;\
	            break;\
	        case '*':\
	            _regex_static_new_at[_curr_at-1].length = REGEX_INF_ZR_LEN;\
	            break;\
	        case '{':{\
	    	    int lp = 0;\
	    	    int _x = _regex_static_new_i+1;\
	    	    for(; _x < sizeof(expr)-1 && expr[_x] != '}'; _x++){\
	    	        lp *=10;\
	    	        lp += expr[_x] - '0';\
	    	    }\
	    	    _regex_static_new_at[_curr_at-1].length = lp;\
	    	    _regex_static_new_i = _x;\
	    	    break;\
	        }\
	        case '[':{\
	            int _c = 0;\
	    	    int _x = _regex_static_new_i+1;\
	    	    for(; _x < sizeof(expr)-1 && expr[_x] != ']'; _x++ ){\
	    	        switch(expr[_x]){\
	    	            case '-':{\
                            int dir = expr[_x-1] < expr[_x+1] ? 1 : -1;\
                            for(uint8_t _j = expr[_x-1]+dir; _j != expr[_x+1]; _j+=dir){\
                                _regex_static_new_push(_curr_at, _j, _c);\
                            }\
                            _regex_static_new_i++;\
	    	    	        break;\
                        }\
	    	    	    case ',':\
	    	    	        break;\
	    	    	    default:\
	    	    	        _regex_static_new_push(_curr_at, expr[_x], _c);\
	    	    	        break;\
	    	        }\
	    	    }\
	    	    _regex_static_new_i = _x;\
	            ret->length++;\
	    	    _curr_at++;\
	    	    break;\
	        }\
            case '\\':\
                switch(expr[_regex_static_new_i+1]){\
                    case 's':\
                        _regex_static_new_set(_curr_at,((uint8_t[]){REGEX_WS_CHARS}),1);\
                        break;\
                    default:\
                        _regex_static_new_set(_curr_at,((uint8_t[]){expr[_regex_static_new_i+1]}),1);\
                        break;\
                }\
                _regex_static_new_i++;\
                break;\
	        default:\
	        	_regex_static_new_set(_curr_at,((uint8_t[]){expr[_regex_static_new_i]}),1);\
	    	break;\
	    }\
    }\
    (ret);\
})
#endif
