#include <kstd.h>
#include <arch/arch.h>
#include <bootloader.h>
#ifdef __x86_64__

__attribute__((aligned(0x10))) static idt_entry_t idt[256] = {0}; // Create an array of IDT entries; aligned for performance
__attribute__((aligned(0x10))) uint64_t idt_stub_table[256] = {0};
static idtr_t idtr;
extern uint64_t isr_stub_table[];

void exception_handler(){
    KPANIC("EXCEPTION HANDLER!!!!!!!");
}
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags){
    idt_entry_t* descriptor = &idt[vector];
 
    descriptor->isr_low       = ((uint64_t)isr+vector) & 0xFFFF;
    descriptor->kernel_cs     = 0x8; //kernel offset in gdt
    descriptor->ist           = 0;
    descriptor->attributes    = flags;
    descriptor->isr_mid       = (((uint64_t)isr+vector) >> 16) & 0xFFFF;
    descriptor->isr_high      = (((uint64_t)isr+vector) >> 32) & 0xFFFFFFFF;
    descriptor->reserved      = 0;
}
void idt_init(){
    for(uint16_t i = 0; i < 256; i++)
        idt_stub_table[i] = (uint64_t)asm_interrupt_handler;
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * 32 - 1;
    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, (void *)idt_stub_table[vector], 0x8E);
    }
    KDEBUG("exception_handler 0x%x", asm_interrupt_handler);
    KDEBUG("idt[0] 0x%x 0x%x 0x%x",
        (uint64_t)idt[0].isr_high,
        (uint64_t)idt[0].isr_mid,
        (uint64_t)idt[0].isr_low
    );
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
