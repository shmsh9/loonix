#ifndef X86_64_H_
#define X86_64_H_
#ifdef __x86_64__
#include <stdint.h>
    typedef struct __attribute__((__packed__)){
        uint64_t rax;
        uint64_t rbx;
        uint64_t rcx;
        uint64_t rdx;
        uint64_t rsi;
        uint64_t rdi;
        uint64_t rbp;
        uint64_t rsp;
        uint64_t r8;
        uint64_t r9;
        uint64_t r10;
        uint64_t r11;
        uint64_t r12;
        uint64_t r13;
        uint64_t r14;
        uint64_t r15;
        uint64_t rip;
        uint64_t xmm0[2];
        uint64_t flags;
    }cpu_registers;
    //https://wiki.osdev.org/GDT
    typedef struct __attribute__((packed)) {
	    uint16_t limit_low;           // The lower 16 bits of the limit.
	    uint16_t base_low;            // Bits 0-15 of the base.
	    uint8_t  base_middle;         // The next 8 bits of the base.
	    uint8_t  access_byte;             // Access flags, determine what ring this segment can be used in.
	    uint8_t  limit_high_and_flags;
	    uint8_t  base_high;           // The next 8 bits of the base.
    } gdt_entry;
    
    typedef struct __attribute__((packed)){
        uint16_t size; //sizeof(gdt_table)-1B
        uint64_t offset;
    } gdt_ptr;

    typedef struct __attribute__((packed)){
    	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
    	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
    	uint8_t	    ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
    	uint8_t     attributes;   // Type and attributes; see the IDT page
    	uint16_t    isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
    	uint32_t    isr_high;     // The higher 32 bits of the ISR's address
    	uint32_t    reserved;     // Set to zero
    }idt_entry_t;

    typedef struct __attribute__((packed)){
    	uint16_t	limit;
    	uint64_t	base;
    } idtr_t;

    typedef struct __attribute__((packed)) _x86_64_interrupt_frame {
        uint64_t  rip;
        uint64_t  cs;
        uint64_t  flags;
        uint64_t  rsp;
    }x86_64_interrupt_frame;

#include <sys/newmem.h>
#include <sys/timer.h>
#include <bootloader.h>
#include <sys/task.h>
    #define INTERRUPT_FUNCTIONS_TABLE_SIZE 256
	#define ARCH_STRING "x64"
    #define ARCH_UINT ARCH_X64
    #define PS2_DEVICE_ADDRESS 0x60
    #define SERIAL_DEVICE_NEW() serial_x86_device_new()
    #define PCI_BUS_ADDRESS 0xCF8
    #define JUMP_INSTRUCTION "jmp"
    #define INTERRUPT_INSTRUCTION "int3"
    #define GET_STACKFRAME(stk) __asm__ __volatile__("mov %%rbp, %0" : "=r"(stk))
    #define CPU_REGISTER_STACK rsp
    #define CPU_REGISTER_FRAME rbp
    #define _INB(address, ret){\
        volatile uint8_t tmp = 0;\
        __asm__ __volatile__("inb %1, %0" :  "=a"(tmp) : "dN"((volatile uint16_t)address));\
        ret = tmp;\
    }
    #define _INW(address, ret){\
        volatile uint16_t tmp = 0;\
        __asm__ __volatile__("inw %1, %0" :  "=a"(tmp) : "dN"((volatile uint16_t)address));\
        ret = tmp;\
    }
    #define _INL(address, ret){\
        volatile uint32_t tmp = 0;\
        __asm__ __volatile__("inl %1, %0" :  "=a"(tmp) : "dN"((volatile uint16_t)address));\
        ret = tmp;\
    }
    #define _OUTB(address, data) __asm__ __volatile__("outb %0, %1" : : "a"((uint8_t)data), "Nd"((uint16_t)address))
    #define _OUTL(address, data) __asm__ __volatile__("outl %0, %1" : : "a"((uint32_t)data), "Nd"((uint16_t)address))
    #define _OUTW(address, data) __asm__ __volatile__("outw %0, %1" : : "a"((uint16_t)data), "Nd"((uint16_t)address))
    #define INTERRUPT_FUNCTIONS_INSTALL_DEFAULT_ARCH(){\
            interrupt_handler_install(interrupt_handler_zerodiv, 0x0);\
            interrupt_handler_install(interrupt_handler_breakpoint, 0x3);\
            interrupt_handler_install(interrupt_handler_invalid_opcode, 0x6);\
            interrupt_handler_install(task_scheduler, 0x20);\
            interrupt_handler_install(interrupt_general_protection_fault, 0xD);\
    }
    /*
    typedef struct __attribute__((packed)) _x86_64_interrupt_frame {
        uint64_t  rip;
        uint64_t  cs;
        uint64_t  flags;
        uint64_t  rsp;
    }x86_64_interrupt_frame;
    */
    #define INTERRUPT_FRAME_PRINT(interrupt_frame){\
        kprintf("\n\t[rip] : 0x%x\n\t[cs ] : 0x%x\n\t[fl ] : 0x%x\n\t[rsp] : 0x%x\n\t\n",\
            ((x86_64_interrupt_frame *)interrupt_frame)->rip,\
            ((x86_64_interrupt_frame *)interrupt_frame)->cs,\
            ((x86_64_interrupt_frame *)interrupt_frame)->flags,\
            ((x86_64_interrupt_frame *)interrupt_frame)->rsp\
        );\
    }
    #define INTERRUPT_INIT(){\
            interrupt_functions_table_init();\
            pic_remap();\
            gdt_ptr *gdt = gdt_entries_new(bootinfo, &heap);\
            gdt_entries_load(gdt);\
            idt_init(bootinfo);\
    }
    #define NEWMEM_HACK_UGLY_OFFSET 0x0
    #define NEWMEM_ALIGN 0x10
    #define VT100_REFRESH_TICK 0x2000000

    #define CPU_REGISTERS_PRINT(regs){\
        char *cpu_registers_names__func__ [sizeof(cpu_registers)/sizeof(uint64_t)] = {0};\
        cpu_registers_names__func__[0] = "rax";\
        cpu_registers_names__func__[1] = "rbx";\
        cpu_registers_names__func__[2] = "rcx";\
        cpu_registers_names__func__[3] = "rdx";\
        cpu_registers_names__func__[4] = "rsi";\
        cpu_registers_names__func__[5] = "rdi";\
        cpu_registers_names__func__[6] = "rbp";\
        cpu_registers_names__func__[7] = "rsp";\
        cpu_registers_names__func__[8] = "r8 ";\
        cpu_registers_names__func__[9] = "r9 ";\
        cpu_registers_names__func__[10] = "r10";\
        cpu_registers_names__func__[11] = "r11";\
        cpu_registers_names__func__[12] = "r12";\
        cpu_registers_names__func__[13] = "r13";\
        cpu_registers_names__func__[14] = "r14";\
        cpu_registers_names__func__[15] = "r15";\
        cpu_registers_names__func__[16] = "rip";\
        cpu_registers_names__func__[17] = "xmm0_0";\
        cpu_registers_names__func__[18] = "xmm0_1";\
        cpu_registers_names__func__[19] = "flags";\
        for(uint8_t i = 0; i < sizeof(cpu_registers)/sizeof(uint64_t); i++){\
            kprintf("\t[%s] : 0x%x\n", cpu_registers_names__func__[i],((uint64_t *)regs)[i]);\
        }\
    }
    void pic_remap();
    void rtc_install();
    void task_interrupt_enable();
    void task_cpu_registers_load(volatile cpu_registers *regs, void *fn, void *data, void *t);
    void task_cpu_registers_reload(volatile cpu_registers *regs);
    void interrupt_enable();
    void interrupt_disable();
    void interrupt_handler_install(void (*fn)(), uint16_t num);
    void interrupt_handler_replace(void (*fn)(), uint16_t num);
    void interrupt_functions_table_init();
    void cpu_registers_save(volatile cpu_registers *regs);
    uint64_t cpu_get_tick();
    void gdt_entries_load(gdt_ptr *ptr);
    gdt_ptr * gdt_entries_new(bootinfo *bi, kheap *heap);
    gdt_entry gdt_entry_new(uint32_t limit, uint64_t base, uint8_t access_byte, uint8_t flags);
    void __memset_64b(void *ptr, uint64_t b, uint64_t sz);
    void __memset_128b(void *ptr, uint64_t b[2], uint64_t sz);
    void __memcpy_64b(void *dst, void *src, uint64_t sz);
    void __memcpy_128b(void *dst, void *src, uint64_t sz);
    void idt_init(bootinfo *bi);
    void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
    /*
    #define __FASTEST_MEMCPY(dst, src, sz) __memcpy_64b(dst, src, sz);
    #define __FASTEST_MEMSET(ptr, b, sz) __memset_64b(ptr, B_to_8B(b), sz);
    */
    #define __FASTEST_MEMCPY(dst, src, sz) __memcpy_128b(dst, src, sz)

    #define __FASTEST_MEMSET(ptr, b, sz) {\
        uint64_t src[2] = {B_to_8B(b), B_to_8B(b)};\
        __memset_128b(ptr, src, sz);\
    }
#endif
#endif
