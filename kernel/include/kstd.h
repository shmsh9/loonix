#ifndef KSTD_H_
#define KSTD_H_
#include <drivers/serial.h>
#include <newmem.h>
#define KERNEL_DEBUG
#ifdef __x86_64__
    #define JMPNOARCH "jmp"
    #define INTNOARCH "int3"
#endif
#ifdef __aarch64__
    #define JMPNOARCH "b"
    #define INTNOARCH "svc #0"
#endif
#define BREAKPOINT() __asm__ __volatile__ ("1: "JMPNOARCH" 1b")
#define INTERRUPT()  __asm__ __volatile__ (INTNOARCH)
#ifdef KERNEL_DEBUG
#define KDEBUG(...) {\
    kprint(__func__);\
    kprint("() : debug : ");\
    kprintf(__VA_ARGS__);\
}
#define KERROR(...) {\
    kprint(__func__);\
    kprint("() : error : ");\
    kprintf(__VA_ARGS__);\
    stacktrace();\
}
#endif
#ifndef KERNEL_DEBUG
#define KDEBUG(...)
#define KERROR(...) {\
    kprint(__func__);\
    kprint("() : error : ");\
    kprintf(__VA_ARGS__);\
}
#endif
#define KALLOC_LIST_MAX 1024
#define STACK_TRACE_NMAX 8

extern uintptr_t __stack_chk_guard;
extern uint32_t kalloc_list_last;
extern kheap_allocated_block kalloc_list[KALLOC_LIST_MAX];
extern kheap heap;

typedef struct {
    uint8_t elementsz;
    uint32_t length;
    uint32_t alloc;
    void *array;  
} karray;

struct stackframe{
    struct stackframe *frame;
    uint64_t instruction_pointer;         
};

void __stack_chk_fail(void);
void stacktrace();
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
int32_t kalloc_find_ptr_alloc(const void *ptr);
void *kmalloc(uint32_t b);
void *kcalloc(uint32_t n, uint32_t sz);
void *krealloc(const void *ptr, uint32_t newsz);
void kfree(void *p);
karray *karray_new(uint8_t elementsz);
void karray_free(karray *array);
void karray_push(karray *array, uint64_t elem);
void karray_print(karray *array);
#endif

