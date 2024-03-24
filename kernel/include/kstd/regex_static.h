#ifndef _REGEX_STATIC_H
#define _REGEX_STATIC_H
#include <kstd/regex.h>
#include <kstd/kstd.h>

#define _REGEX_STATIC_ANY \
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,\
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,\
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,\
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,\
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,\
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,\
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,\
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,\
    123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,\
    136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,\
    149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,\
    162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,\
    175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,\
    188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,\
    201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,\
    214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,\
    227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,\
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,\
    253, 254, 255\

#define _REGEX_STATIC_0_9 '0','1','2','3','4','5','6','7','8','9'
#define _REGEX_STATIC_a_z 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
#define _REGEX_STATIC_A_Z 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
#define _REGEX_STATIC_ALPHA _REGEX_STATIC_A_Z,_REGEX_STATIC_a_z
#define _REGEX_STATIC_ALPHANUM _REGEX_STATIC_ALPHA,_REGEX_STATIC_0_9
#define _REGEX_STATIC_WS REGEX_WS_CHARS

#define _regex_dict(...) __VA_ARGS__
#define _regex_automaton_static(a,l) ({\
    (uint64_t)&(regex_automaton){\
        .length = l,\
        .alphabet = _karray_static(((char[])a))\
    };\
})
#define _regex_static_arr_contains(arr, c) _regex_static_arr_contains_internal(arr, sizeof(arr), c)
static inline bool _regex_static_arr_contains_internal(char arr[], int l, char c){
    for(int i = 0; i < l; i++)
        if(arr[i] == c)
            return true;
    return false;
} 
#define _regex_static(...) _karray_static( ((uint64_t[]){ __VA_ARGS__ }) )
#define _regex_static_new_set(i,a,n) \
    for(int _regex_static_new_set_i = 0;_regex_static_new_set_i < sizeof(a)/sizeof(a[0]);_regex_static_new_set_i++)\
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
    _regex_static_new_arr_raw[i][n] = c;\
    _regex_static_new_arr[i] = (karray){ \
        .length = n, .alloc = UINT8_MAX, \
        .array = _regex_static_new_arr_raw+i, \
        .elementsz = sizeof(c) \
    }; \
    _regex_static_new_at[i] = (regex_automaton){.length = 1, .alphabet = _regex_static_new_arr+i};\
    _regex_static_new_ret[i] = (uint64_t)(_regex_static_new_at+i);\
    n++;\

#define _regex_static_new(expr) ({\
    static uint64_t _regex_static_new_ret[sizeof(expr)-1] = {0};\
    static char _regex_static_new_arr_raw[sizeof(expr)-1][UINT8_MAX] = {0};\
    static karray _regex_static_new_arr[sizeof(expr)-1] = {0}; \
    static regex_automaton _regex_static_new_at[sizeof(expr)-1] = {0};\
    karray *ret = _karray_static(_regex_static_new_ret);\
    ret->length = 0;\
    int _curr_at = 0;\
    for(int _regex_static_new_i = 0; _regex_static_new_i < sizeof(expr)-1; _regex_static_new_i++){\
        if(!_regex_static_arr_contains( ((char[]){REGEX_SPECIAL_CHARS}), expr[_regex_static_new_i])) {\
            _regex_static_new_set(_curr_at,((char[]){expr[_regex_static_new_i]}),1);\
	    continue;\
	}\
	switch(expr[_regex_static_new_i]){\
	    case '.':\
                _regex_static_new_set(_curr_at,((char[]){_REGEX_STATIC_ANY}),1);\
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
		        case '-':\
			    break;\
			case ',':\
			    break;\
			default:\
			    \
			    _regex_static_new_push(_curr_at, expr[_x], _c);\
			    break;\
		    }\
		}\
		_regex_static_new_i = _x;\
	        ret->length++;\
		_curr_at++;\
		break;\
	    }\
	    default:\
	    	\
		break;\
	}\
    }\
    (ret);\
})
#endif
