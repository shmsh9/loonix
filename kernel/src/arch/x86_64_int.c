#include <kstd.h>
#include <timer.h>
#include <arch/arch.h>
#include <bootloader.h>
#ifdef __x86_64__

__attribute__((aligned(0x10))) static idt_entry_t idt[256] = {0}; // Create an array of IDT entries; aligned for performance
static idtr_t idtr;
extern uint64_t asm_interrupt_table[256];
uint8_t serial_device_char_in = 0;
extern uint64_t interrupt_functions_table[256];

void interrupt_handler(uint64_t rdi_interrupt_num, x86_64_interrupt_frame *frame){
    if(interrupt_functions_table[rdi_interrupt_num]){
        ((void (*)(x86_64_interrupt_frame *))interrupt_functions_table[rdi_interrupt_num])(frame);
        return;
    }
    KPANIC("unhandled exception (%d) :\n\t[rip]    : 0x%x\n\t[rsp]    : 0x%x\n\t[rflags] : 0b%b\n\t[cs]     : 0x%x\n", 
        rdi_interrupt_num,
        frame->rip,
        frame->rsp,
        (uint64_t)frame->flags,
        frame->cs
    );
}
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags){
    idt_entry_t* descriptor = &idt[vector];
 
    descriptor->isr_low       = ((uint64_t)isr) & 0xFFFF;
    descriptor->kernel_cs     = 0x8; //kernel offset in gdt
    descriptor->ist           = 0;
    descriptor->attributes    = flags;
    descriptor->isr_mid       = (((uint64_t)isr) >> 16) & 0xFFFF;
    descriptor->isr_high      = (((uint64_t)isr) >> 32) & 0xFFFFFFFF;
    descriptor->reserved      = 0;
}
void pic_remap(){
    interrupt_disable();
    uint16_t PIC1_COMMAND = 0x20;
    uint16_t PIC2_COMMAND = 0xA0;
    uint16_t PIC1_DATA = PIC1_COMMAND+1;
    uint16_t PIC2_DATA = PIC2_COMMAND+1;
    uint8_t mask1, mask2;
    //save mask
    mask1 = inb(PIC1_DATA);
    mask2 = inb(PIC2_DATA);
    //restart
    outb(PIC1_COMMAND,0x11);
    outb(PIC2_COMMAND,0x11);
    //remap
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);
    //cascading
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    //end
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    //reset mask
    //outb(PIC1_DATA, 0xff);
    //outb(PIC2_DATA, 0xff);
}
void idt_init(bootinfo *bi){
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;
    for(uint16_t vector = 0; vector < 256; vector++){
        idt_set_descriptor(
            vector,
            (void *)((uint64_t)asm_interrupt_table[vector]+(uint64_t)bi->kernelbase), 
            0x8E
        );
    }
    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag

}
gdt_entry gdt_entry_new(uint32_t limit, uint64_t base, uint8_t access_byte, uint8_t flags){
    /*
    if(limit > 0xfffff){
        KERROR("limit cannot exceed 20bits");
        return (gdt_entry){0};
    }
    */
    uint8_t limit_high_and_flags = (limit >> 16) & 0x0f;
    return (gdt_entry){
        .base_low = base & 0xffff,
        .base_middle = (base >> 16) & 0xff,
        .base_high = (base >> 24) & 0xff,
        .limit_low = limit & 0xffff,
        .limit_high_and_flags = limit_high_and_flags | (flags & 0xf0),
        .access_byte = access_byte
    };
}
gdt_ptr * gdt_entries_new(bootinfo *bi, kheap *heap){
    uint8_t gdt_n_entries = 3;
    gdt_entry *entries = kcalloc(sizeof(gdt_entry),gdt_n_entries);
    gdt_ptr *ret = kcalloc(sizeof(gdt_ptr),1);
    ret->offset = (volatile uint64_t)entries;
    ret->size = (gdt_n_entries*sizeof(gdt_entry))-1;
    //Null descriptor
    gdt_entry zero_entry = {0};
    entries[0] = zero_entry;
    //Kernel mode code
    entries[1] = gdt_entry_new(
        0xfffff, 
        0,
        0x9a,
        0xaf
    );
    //Kernel mode data
    entries[2] = gdt_entry_new(
        0xfffff,
        0,
        0x92,
        0xaf
    );
    /*
    //Task state segment
    ret[3] = gdt_entry_new(

    );
    */
    return ret;
}
#endif
