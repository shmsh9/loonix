#ifndef KSTD_H_
#define KSTD_H_
#include <drivers/serial.h>
#include <drivers/framebuffer.h>
#include <newmem.h>
#include <bootloader.h>
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
#define BYTES_TO_MB(b) ((b) >> 20)
#define MB_TO_BYTES(mb) ((mb) << 20) 
#define KMSG(type, ...) {\
    runtime_services->GetTime(&global_efi_time, 0);\
    kprintf("[kernel][%s][%d:%d:%d] : %s() : ", type, global_efi_time.hour, global_efi_time.minute, global_efi_time.second,__func__);\
    kprintf(__VA_ARGS__);\
    kputc('\n');\
}
#define KMESSAGE(...) KMSG("message", __VA_ARGS__)
#define KERROR(...) {\
    KMSG("error", __VA_ARGS__);\
    stacktrace();\
}
#ifdef KERNEL_DEBUG
#define KDEBUG(...) KMSG("debug", __VA_ARGS__)
#endif
#ifndef KERNEL_DEBUG
#define KDEBUG(...)
#endif
#define KALLOC_LIST_MAX 1024
#define STACK_TRACE_NMAX 8

extern uintptr_t __stack_chk_guard;
extern uint32_t kalloc_list_last;
extern kheap_allocated_block kalloc_list[KALLOC_LIST_MAX];
extern kheap heap;
extern efi_runtime_services *runtime_services;
extern struct efi_time global_efi_time;

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
void __fast_zeromem(void *ptr, uint64_t sz);
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
void *kmalloc(uint64_t b);
void *kcalloc(uint64_t n, uint64_t sz);
void *krealloc(const void *ptr, uint64_t newsz);
void kfree(void *p);
karray *karray_new(uint8_t elementsz);
void karray_free(karray *array);
void karray_push(karray *array, uint64_t elem);
void karray_print(karray *array);
#endif

