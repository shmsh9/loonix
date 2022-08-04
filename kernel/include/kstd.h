#ifndef KSTD_H_
#define KSTD_H_
#include <drivers/serial.h>
#include <mem.h>
#ifdef __x86_64__
    #define JMPNOARCH "jmp"
#endif
#ifdef __aarch64__
    #define JMPNOARCH "b"
#endif
#define BREAKPOINT() {\
    __asm__ __volatile__ ("1: "JMPNOARCH" 1b");\
}
int strlen(const char *str);
int strcmp(const char *str1, const char *str2);
char *strdup(const char *str);
void memset(void *ptr, uint8_t b, uint64_t sz);
void kprintf(const char *fmt, ...);
void kprint(const char *str);
void memcpy(void *dst, const void *src, uint64_t sz);
char kgetchar();
void kputc(uint8_t c);
void *kmalloc(uint32_t b);
void kfree(void *p);
#endif

