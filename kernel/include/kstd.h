#ifndef KSTD_H_
#define KSTD_H_
#include <serial.h>
int strlen(const char *str);
void memset(void *ptr, uint8_t b, uint64_t sz);
void kprintf(const char *fmt, ...);
void kprint(const char *str);
void memcpy(void *dst, const void *src, uint64_t sz);
#endif
