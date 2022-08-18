#include <drivers/ps2.h>
#include <drivers/serial.h>
#include <newmem.h>
#include <kernel.h>
#include <bootloader.h>
#include <arch/aarch64.h>
#include <arch/x86_64.h>
#include <acpi.h>
#include <irq/irq.h>

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;
kheap_allocated_block *kalloc_list = 0;
kheap_allocated_block kalloc_list_block = {0};
kheap heap;
framebuffer_device fb = {0};
uint32_t kalloc_list_last = 0;
efi_runtime_services *runtime_services = 0;
char ** font8x8 = {0};
ps2_device ps2 = {0};
serial_device serial;

void crt0(struct bootinfo *bootinfo){
    //Heap allocation not allowed until said otherwise
    irq_disable();
    serial = serial_device_new();
    runtime_services = bootinfo->RuntimeServices;
    if(bootinfo->uefi_exit_code)
        KPANIC("uefi_exit_code returned 0x%x", bootinfo->uefi_exit_code);
    if(!bootinfo->mmap)
        KPANIC("cannot retrieve memory map at 0x%x !", bootinfo->mmap);
    
    mmap mmap = mmap_new(bootinfo);
    acpi_table acpi_table = acpi_table_new(&mmap);
    KDEBUG("acpi_table 0x%x", acpi_table.acpi_memory);
    struct efi_memory_descriptor *largest_mem_block = mmap_find_largest_block(&mmap);
    if(!largest_mem_block)
        KPANIC("no available memory found !");

    KDEBUG("largest mem block : 0x%x %dMB (%d pages)", 
            largest_mem_block->physical_start, 
            BYTES_TO_MB(largest_mem_block->pages*HEAP_BLOCK_SIZE),
            largest_mem_block->pages
    );
    uintptr_t ram_addr = largest_mem_block->physical_start;
    uint64_t ram_pages_n = largest_mem_block->pages;
    if(ram_addr == (uintptr_t)bootinfo->kernelbase || ram_addr <= (uintptr_t)bootinfo->kernelbase+bootinfo->kernelsize){
        KDEBUG("protecting kernel also at 0x%x", bootinfo->kernelbase);
        ram_addr += bootinfo->kernelsize+NEWMEM_HACK_UGLY_OFFSET;
        //ram_pages_n -= (bootinfo->kernelsize+HACK_UGLY_OFFSET / HEAP_BLOCK_SIZE)+1;
    }
    kheap_init(&heap);
    kheap_add_blocks(&heap, ram_addr, ram_pages_n);
    kalloc_list_block = kheap_get_free_mem2(&heap, sizeof(kheap_allocated_block)*KALLOC_LIST_START_ALLOC);
    kalloc_list = (kheap_allocated_block *)kalloc_list_block.ptr;
    //It is allowed to do heap allocations after this line
    //!\ contiguous memory is needed
    fb = framebuffer_new_device(
        bootinfo->framebuffer.address, 
        bootinfo->framebuffer.width, 
        bootinfo->framebuffer.height, 
        bootinfo->framebuffer.size, 
        FRAMEBUFFER_DOUBLE_BUFFERING);
	framebuffer_clear(&fb, &(graphics_pixel){.Red = 0x00, .Green = 0x00, .Blue = 0x00, .Alpha = 0xff});
    font8x8 = font8x8_new();
    ps2 = ps2_device_new(PS2_DEVICE_ADDRESS);
    kmain(bootinfo);
}
