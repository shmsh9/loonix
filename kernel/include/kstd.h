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
#define STACK_GUARD_START() uintptr_t canary = __stack_chk_guard
#define STACK_GUARD_STOP() {if ( (canary = canary ^ __stack_chk_guard) != 0 )\
    {__stack_chk_fail();}\
}
extern uintptr_t __stack_chk_guard;

typedef struct {
    uint8_t elementsz;
    uint32_t length;
    uint32_t alloc;
    void *array;  
} karray;
void __stack_chk_fail(void);
int strlen(const char *str);
int strcmp(const char *str1, const char *str2);
char *strdup(const char *str);
void memset(void *ptr, uint8_t b, uint64_t sz);
void kprintf(const char *fmt, ...);
void kprint(const char *str);
void memcpy(void *dst, const void *src, uint64_t sz);
int memcmp(const void *ptr1, const void *ptr2, uint64_t sz);
char kgetchar();
void kputc(uint8_t c);
void *kmalloc(uint32_t b);
void *kcalloc(uint32_t n, uint32_t sz);
void *krealloc(const void *ptr, uint32_t oldsz , uint32_t newsz);
void kfree(void *p);
karray *karray_new(uint8_t elementsz);
void karray_free(karray *array);
void karray_push(karray *array, uint64_t elem);
void karray_print(karray *array);
#endif

