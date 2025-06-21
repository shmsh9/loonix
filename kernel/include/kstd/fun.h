#ifndef _FUN_H_
#define _FUN_H_

#define PARENS ()
#define RECURSE(...)  RECURSE4(RECURSE4(RECURSE4(RECURSE4(__VA_ARGS__))))
#define RECURSE4(...) RECURSE3(RECURSE3(RECURSE3(RECURSE3(__VA_ARGS__))))
#define RECURSE3(...) RECURSE2(RECURSE2(RECURSE2(RECURSE2(__VA_ARGS__))))
#define RECURSE2(...) RECURSE1(RECURSE1(RECURSE1(RECURSE1(__VA_ARGS__))))
#define RECURSE1(...) __VA_ARGS__

#define FOREACH(F,...) __VA_OPT__(RECURSE(_FOREACH(F, __VA_ARGS__)))
#define _FOREACH(F,E,...) F(E) __VA_OPT__(__FOREACH PARENS (F, __VA_ARGS__))
#define __FOREACH() _FOREACH


#define _CAT(A,B) A ## B 
#define CAT(A,B) _CAT(A,B)
#define ARRAY_ID(X) X,
#define ARRAY(...) FOREACH(ARRAY_ID, __VA_ARGS__)
#define STR(X) #X
#define ENUMDEF(N,...) enum N {ARRAY(__VA_ARGS__)}
#define _STRENUMDEF(E) case E: return STR(E);
#define STRENUMDEF(N,...) \
const char * CAT(N,_to_str)(enum N n){ \
	switch(n){ \
		FOREACH(_STRENUMDEF, __VA_ARGS__)\
		default: return NULL;\
	} \
}
#define ENUM(N,...) \
	ENUMDEF(N, __VA_ARGS__); \
	STRENUMDEF(N, __VA_ARGS__)

#endif

