#include <kstd.h>
#include <arch/arch.h>
#include <bootloader.h>
#ifdef __x86_64__
gdt_entry gdt_entry_new(uint32_t limit, uint32_t base, uint8_t access_byte, uint8_t flags){
    if(limit > 0xfffff){
        KERROR("limit cannot exceed 20bits");
        return (gdt_entry){0};
    }
    uint8_t limit_high_and_flags = (limit >> 16) & 0x0f;
    limit_high_and_flags |= (flags << 4);
    return (gdt_entry){
        .base_low = base & 0xff,
        .base_middle = (base >> 8) & 0xff,
        .base_high = (base >> 16) & 0xffff,
        .limit_low = limit & 0xffff,
        .limit_high_and_flags = limit_high_and_flags,
        .access_byte = access_byte
    };
}
gdt_ptr * gdt_entries_new(bootinfo *bi, kheap *heap){
    gdt_entry *entries = kcalloc(sizeof(gdt_entry),3);
    gdt_ptr *ret = kcalloc(sizeof(gdt_ptr),1);
    ret->offset = (uint64_t)entries;
    ret->size = 3*(sizeof(gdt_entry))-1;
    KDEBUG("ret->offset 0x%x", ret->offset);
    KDEBUG("ret->size 0x%x", ret->size);
    //Null descriptor
    entries[0] = (gdt_entry){
        .base_high = 0x0,
        .base_middle = 0x0,
        .base_low = 0x0,
        .limit_low = 0x0,
        .limit_high_and_flags = 0x0,
        .access_byte = 0x0
    };
    //Kernel mode code
    entries[1] = gdt_entry_new(
        bi->kernelsize, 
        ((uint64_t)bi->kernelbase & 0xffff),
        0x9a,
        0xc
        );
    //Kernel mode data
    entries[2] = gdt_entry_new(
        0xeffff,
        ((uint64_t)bi->kernelbase+bi->kernelsize) & 0xffff,
        0x92,
        0xc
    );
    /*
    //Task state segment
    ret[3] = gdt_entry_new(

    );
    */
    return ret;
}
#endif
