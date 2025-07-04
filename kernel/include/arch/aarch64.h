#ifndef AARCH64_H_
#define AARCH64_H_
#include <stdint.h>
#ifdef __aarch64__
    #define INTERRUPT_FUNCTIONS_TABLE_SIZE 256
    #define CPU_REGISTER_STACK sp
	#define ARCH_STRING "aarch64"
    #define ARCH_UINT ARCH_AARCH64
    #define SERIAL_DEVICE_NEW() serial_pl011_device_new()
    #define PS2_DEVICE_ADDRESS 0x0 //no ps/2 port for Virt
    #define PCI_BUS_ADDRESS 0x0
    #define JUMP_INSTRUCTION "b"
    #define INTERRUPT_INSTRUCTION "svc #0"
    #define GET_STACKFRAME(stk) __asm__ __volatile__("mov %0, x29" : "=r"(stk))
    #define _INB(address, ret) ret = *(volatile uint8_t *)address
    #define _INW(addess, ret) ret = *(volatile uint16_t *)address
    #define _INL(addess, ret) ret = *(volatile uint32_t *)address
    #define _OUTB(address, data) *(volatile uint8_t *)address = data
    #define _OUTL(address, data) *(volatile uint32_t *)address = data
    #define _OUTW(address, data) *(volatile uint16_t *)address = data
    #define NEWMEM_HACK_UGLY_OFFSET 0
    #define NEWMEM_ALIGN 0x10
    #define VT100_REFRESH_TICK 0x200000
    #define INTERRUPT_FUNCTIONS_INSTALL_DEFAULT_ARCH(){\
        interrupt_handler_install(interrupt_handler_breakpoint, 0x0);\
    }    

    #define INTERRUPT_FRAME_PRINT(interrupt_frame){\
    }
    #define INTERRUPT_INIT(){\
        interrupt_functions_table_init();\
        init_interrupt_vector_table();\
    }
    typedef struct __attribute__((packed)){
        uint64_t x0;
        uint64_t x1;
        uint64_t x2;
        uint64_t x3;
        uint64_t x4;
        uint64_t x5;
        uint64_t x6;
        uint64_t x7;
        uint64_t x8;
        uint64_t x9;
        uint64_t x10;
        uint64_t x11;
        uint64_t x12;
        uint64_t x13;
        uint64_t x14;
        uint64_t x15;
        uint64_t x16;
        uint64_t x17;
        uint64_t x18;
        uint64_t x19;
        uint64_t x20;
        uint64_t x21;
        uint64_t x22;
        uint64_t x23;
        uint64_t x24;
        uint64_t x25;
        uint64_t x26;
        uint64_t x27;
        uint64_t x28;
        uint64_t x29;
        uint64_t x30;
        uint64_t sp;
    }cpu_registers;
    #define CPU_REGISTERS_PRINT(regs){\
        for(uint8_t i = 0; i < (sizeof(cpu_registers)/sizeof(uint64_t)) - 1; i++){\
            kprintf("\t[x%d%s] : 0x%x\n", i, i < 10 ? " " : "" ,((uint64_t *)regs)[i]);\
        }\
        kprintf("\t[sp ] : 0x%x\n", ((uint64_t *)regs)[31]);\
    }
    typedef struct __attribute__((packed)) _interrupt_vector_table_entry{
        uint64_t a;
        uint64_t b;
        uint64_t c;
        uint64_t d;
        uint64_t e;
        uint64_t f;
        uint64_t g;
        uint64_t h;
        uint64_t i;
        uint64_t j;
        uint64_t k;
        uint64_t l;
        uint64_t m;
        uint64_t n;
        uint64_t o;
        uint64_t p;

    }interrupt_vector_table_entry;
    typedef struct _esr_el1{
        uint8_t direction_bit; //0
        uint8_t CRm;  //4:1
        uint8_t RT;   //9:5
        uint8_t RT2;  //14:10
        uint8_t OPC1; //19:16
        uint8_t COND; //23:20
        uint8_t CV;  //24
        uint32_t ISS; //24:0
    }esr_el1;
    void interrupt_enable();
    void interrupt_disable();
    void interrupt_functions_table_init();
    void interrupt_handler_install(void (*fn)(), uint16_t num);
    void init_interrupt_vector_table();
    void interrupt_handler(uint64_t far_el1, uint64_t esr_el1, uint64_t interrupt_type);
    void task_cpu_registers_load(volatile cpu_registers *regs, void(*fn)(void *, struct _task *), void *data, void *t);
    void task_cpu_registers_reload(volatile cpu_registers *regs);
    void cpu_registers_save(volatile cpu_registers *regs);
    void cpu_registers_load(volatile cpu_registers *regs);
    uint64_t cpu_get_tick();
    uint64_t cpu_get_cntpct();
    void __memset_64b(void *ptr, uint64_t b, uint64_t sz);
    void __memset_128b(void *ptr, uint64_t b[2], uint64_t sz);
    void __memcpy_64b(void *dst, void *src, uint64_t sz);
    void __memcpy_128b(void *dst, void *src, uint64_t sz);
    #define __FASTEST_MEMCPY(dst, src, sz) __memcpy_128b(dst, src, sz)
    #define __FASTEST_MEMSET(ptr, b, sz) {\
        uint64_t __FASTEST_MEMSET_SRC##__COUNTER__[2] = {B_to_8B(b), B_to_8B(b)};\
        __memset_128b(ptr, __FASTEST_MEMSET_SRC##__COUNTER__, sz);\
    }
#endif
#endif
