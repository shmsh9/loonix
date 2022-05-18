#ifndef STRING_H_
#define STRING_H_
#include <type.h>
#include <stdlib_h.h>
void memset(void *dst, char val, size_t sz);
void memcpy(void *dst, void *src, size_t sz);
size_t strlen(const CHAR16 *str);
int strcmp(const CHAR16 *str1, const CHAR16 *str2);
CHAR16 *strcpy(CHAR16 *dst, const CHAR16 *src);
CHAR16 *strcat(CHAR16 *dst, const CHAR16 *src);
CHAR16 *strdup(const CHAR16 *src);
#endif
