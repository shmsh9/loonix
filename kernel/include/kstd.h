#ifndef KSTD_H_
#define KSTD_H_
#include <drivers/serial.h>
#include <drivers/framebuffer.h>
#include <drivers/ps2.h>
#include <drivers/vt100.h>
#include <graphics/font8x8.h>
#include <newmem.h>
#include <bootloader.h>
#include <arch/aarch64.h>
#include <arch/x86_64.h>

#define KERNEL_DEBUG
#define BREAKPOINT() __asm__ __volatile__ ("1: "JUMP_INSTRUCTION" 1b")
#define INTERRUPT()  __asm__ __volatile__ (INTERRUPT_INSTRUCTION)
#define BYTES_TO_MB(b) ((b) >> 20)
#define BYTES_TO_KB(b) ((b) >> 10)
#define MB_TO_BYTES(mb) ((mb) << 20) 
#define KB_TO_BYTES(kb) ((kb) << 10)

#define KMSG(type, ...) {\
    kprintf("[kernel][%s] : %s() : ", type, __func__);\
    kprintf(__VA_ARGS__);\
    kputc('\n');\
}
#define KMESSAGE(...) KMSG("message", __VA_ARGS__)
#define KERROR(...) {\
    KMSG("error", __VA_ARGS__);\
    kprintf("%s:%d\n", __FILE__, __LINE__);\
    stacktrace();\
}
#define KPANIC(...){\
    KMSG("kernel panic !", __VA_ARGS__);\
    kprintf("%s:%d\n", __FILE__, __LINE__);\
    stacktrace();\
    BREAKPOINT();\
}
#ifdef KERNEL_DEBUG
//#define KDEBUG(...) KMSG("debug", __VA_ARGS__)
#define KDEBUG(fmt, ...) {\
    kprintf("[kernel][debug] : %s() : "fmt"\n", __func__, __VA_ARGS__);\
}
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
extern framebuffer_device fb;
extern uint64_t framebuffer_text_current_x;
extern uint64_t framebuffer_text_current_y;
extern char ** font8x8;
extern ps2_device ps2;
extern serial_device serial;

typedef struct {
    uint8_t elementsz;
    uint32_t length;
    uint32_t alloc;
    void(*karray_data_free_fn)(void *);
    void *array;
} karray;

typedef struct _klist{
    struct _klist_element *first;
    struct _klist_element *last;
    void(*klist_data_free_fn)(void *);
} klist;

typedef struct _klist_element{
    struct _klist_element *next;
    struct _klist_element *prev;
    uintptr_t data;
}klist_element;

struct stackframe{
    struct stackframe *frame;
    uint64_t instruction_pointer;         
};
void __stack_chk_fail(void);
void __fast_zeromem(void *ptr, uint64_t sz);
void stacktrace();
int strlen(const char *str);
int strcmp(const char *str1, const char *str2);
int atoi(const char *str);
int isdigit(uint8_t c);
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
karray *karray_new(uint8_t elementsz, void(*karray_data_free_fn)(void *));
void karray_free(karray *array);
void karray_push(karray *array, uint64_t elem);
void karray_debug_print(karray *array);
klist *klist_new(uintptr_t data, void(*klist_data_free_fn)(void *));
void klist_push(klist *k, uintptr_t data);
void klist_free(klist *k);
void klist_debug_print(klist *k);
#endif

