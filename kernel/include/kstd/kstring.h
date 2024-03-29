#ifndef KSTRING_H_
#define KSTRING_H_
#include <stdint.h>
#include <kstd/kstd.h>

int strlen(const char *str);
int strcmp(const char *str1, const char *str2);
int atoi(const char *str);
char *itoa(int64_t v);
uint8_t toupper(uint8_t c);
uint8_t tolower(uint8_t c);
int isdigit(uint8_t c);
char *strdup(const char *str);
void memset(void *ptr, uint8_t b, uint64_t sz);
void memcpy(void *dst, const void *src, uint64_t sz);
uint64_t B_to_8B(uint8_t b);
char *strings_join(char **s, int n, char j);
void string_replace(char *s, char c, char r);
#endif
