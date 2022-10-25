#ifndef KSTD_H_
#define KSTD_H_
#include <drivers/serial.h>
#include <drivers/framebuffer.h>
#include <drivers/ps2.h>
#include <drivers/vt100.h>
#include <sys/newmem.h>
#include <bootloader.h>
#include <arch/arch.h>
#include <sys/timer.h>
#include <kstd/kstring.h>
#include <kstd/karray.h>
#include <kstd/klist.h>
#include <kstd/ktime.h>
#include <sys/task.h>

#define KERNEL_DEBUG
#define BREAKPOINT() __asm__ __volatile__ ("1: "JUMP_INSTRUCTION" 1b")
#define INTERRUPT()  __asm__ __volatile__ (INTERRUPT_INSTRUCTION)
#define BYTES_TO_MB(b) ((b) >> 20)
#define BYTES_TO_KB(b) ((b) >> 10)
#define MB_TO_BYTES(mb) ((mb) << 20) 
#define KB_TO_BYTES(kb) ((kb) << 10)
#define KALLOC_LIST_START_ALLOC 1024
#define STACK_TRACE_NMAX 10

#define STACKTRACE() {\
    struct stackframe *stk = {0};\
    GET_STACKFRAME(stk);\
    kprint("stacktrace : \n");\
    for(uint8_t i = 0; stk && i < STACK_TRACE_NMAX; i++){\
        kprintf("\t[%d] : 0x%x\n", i, stk->instruction_pointer);\
        stk = stk->frame;\
    }\
}
#define STACKTRACE_CTXT(ctxt) {\
    struct stackframe *stk = (struct stackframe *)ctxt;\
    kprint("stacktrace : \n");\
    for(uint8_t i = 0; stk && i < STACK_TRACE_NMAX; i++){\
        kprintf("\t[%d] : 0x%x\n", i, stk->instruction_pointer);\
        stk = stk->frame;\
    }\
}
#define KMSG(type, ...) {\
    kprintf("[%d][kernel][%s] : %s() : ",cpu_get_tick(),type, __func__);\
    kprintf(__VA_ARGS__);\
    kputc('\n');\
}
#define KMESSAGE(...) KMSG("message", __VA_ARGS__)
#define KERROR(...) {\
    KMSG("error", __VA_ARGS__);\
    kprintf("%s:%d\n", __FILE__, __LINE__);\
    STACKTRACE();\
    framebuffer_device_update(fb);\
}
#define KPANIC(...){\
	volatile cpu_registers r__func__;\
	cpu_registers_save(&r__func__);\
    KMSG("panic !", __VA_ARGS__);\
    kprintf("%s:%d\n", __FILE__, __LINE__);\
    STACKTRACE();\
    kprint("registers  :\n");\
	CPU_REGISTERS_PRINT(&r__func__);\
    framebuffer_device_update(fb);\
    BREAKPOINT();\
}
#ifdef KERNEL_DEBUG
#define KDEBUG(fmt, ...) {\
    kprintf("[%d][kernel][debug] : %s() %s:%d : "fmt"\n",cpu_get_tick(),__func__,__FILE__,__LINE__, __VA_ARGS__);\
}
#endif
#ifndef KERNEL_DEBUG
#define KDEBUG(...)
#endif
#define MEMORY_LEAK_TEST_START() heap.free_memory
#define MEMORY_LEAK_TEST_STOP(mem){\
    uint64_t newmem = heap.free_memory;\
    if(mem < newmem){\
        KERROR("memory leaked ;( start : %d stop : %d", mem, newmem)\
    }\
    else{\
      KMESSAGE("no memory leak =)")\
    }\
}


struct stackframe{
    struct stackframe *frame;
    uint64_t instruction_pointer;         
};

typedef karray event_loop;
extern uintptr_t __stack_chk_guard;
extern kheap_allocated_block *kalloc_list;
extern kheap_allocated_block kalloc_list_block;
extern kheap heap;
extern EFI_RUNTIME_SERVICES *runtime_services;
extern struct efi_time global_efi_time;
extern framebuffer_device *fb;
extern uint64_t framebuffer_text_current_x;
extern uint64_t framebuffer_text_current_y;
extern ps2_device *ps2;
extern serial_device *serial;
extern event_loop *main_event_loop;

void __stack_chk_fail(void);
void stacktrace();
int strlen(const char *str);
int strcmp(const char *str1, const char *str2);
int atoi(const char *str);
uint8_t toupper(uint8_t c);
uint8_t tolower(uint8_t c);
int isdigit(uint8_t c);
char *strdup(const char *str);
void memset(void *ptr, uint8_t b, uint64_t sz);
void kprintf(const char *fmt, ...);
void kprinthex(void *ptr, uint64_t n);
void kprint(const char *str);
void memcpy(void *dst, const void *src, uint64_t sz);
int memcmp(const void *ptr1, const void *ptr2, uint64_t sz);
char kgetchar();
char kgetchar_non_blocking();
void kputc(uint8_t c);
int32_t kalloc_find_ptr_alloc(const void *ptr);
void *kmalloc(uint64_t b);
void *kcalloc(uint64_t n, uint64_t sz);
void *krealloc(const void *ptr, uint64_t newsz);
void kfree(void *p);
#endif

