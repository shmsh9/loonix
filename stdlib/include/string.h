#ifndef STRING_H_
#define STRING_H_
#include "type.h"
void memset(void *dst, char val, size_t sz);
void memcpy(void *dst, void *src, size_t sz);
size_t strlen(CHAR16 *str);
#endif
