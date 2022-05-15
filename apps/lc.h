#ifndef LC_H_
#define LC_H_
#include <stdint.h>
typedef uintptr_t size_t;
void memset(void *dst, char val, size_t sz);
void memcpy(void *dst, void *src, size_t sz);

#endif
