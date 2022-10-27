#include <drivers/ps2.h>
#include <drivers/serial.h>
#include <drivers/pci.h>
#include <drivers/rtc.h>
#include <sys/newmem.h>
#include <kernel.h>
#include <bootloader.h>
#include <arch/arch.h>
#include <drivers/acpi.h>
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;
kheap_allocated_block *kalloc_list = 0;
kheap_allocated_block kalloc_list_block = {0};
kheap heap;
framebuffer_device *fb = 0x0;
EFI_RUNTIME_SERVICES *runtime_services = 0;
ps2_device *ps2 = 0x0;
serial_device *serial = 0x0;
acpi_table *acpi_tables = 0x0;

void crt0(bootinfo *bootinfo){
    //Heap allocation not allowed until said otherwise
    interrupt_disable();
    runtime_services = bootinfo->RuntimeServices;
    if(bootinfo->uefi_exit_code)
        KPANIC("uefi_exit_code returned 0x%x", bootinfo->uefi_exit_code);
    if(!bootinfo->mmap)
        KPANIC("cannot retrieve memory map at 0x%x !", bootinfo->mmap);
    mmap mmap = mmap_new(bootinfo);
    EFI_MEMORY_DESCRIPTOR *largest_mem_block = mmap_find_largest_block(&mmap);
    if(!largest_mem_block)
        KPANIC("no available memory found !");

    KDEBUG("largest mem block : 0x%x %dMB (%d pages)", 
            largest_mem_block->PhysicalStart,
            BYTES_TO_MB(largest_mem_block->NumberOfPages*HEAP_BLOCK_SIZE),
            largest_mem_block->NumberOfPages
    );
    uintptr_t ram_addr = largest_mem_block->PhysicalStart;
    uint64_t ram_pages_n = largest_mem_block->NumberOfPages;
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
    //process_init();
    //timer_init();
    //rtc_device_init();
    fb = framebuffer_device_new(
        bootinfo->framebuffer.address, 
        bootinfo->framebuffer.width, 
        bootinfo->framebuffer.height, 
        FRAMEBUFFER_DOUBLE_BUFFERING
    );
	framebuffer_device_clear(
        fb, 
        &(graphics_pixel){
            .Red = 0x00, 
            .Green = 0x00, 
            .Blue = 0x00, 
            .Alpha = 0xff
    });
    vt100_console_init(fb);
    serial = SERIAL_DEVICE_NEW();
    INTERRUPT_INIT();
    ps2 = ps2_device_new(PS2_DEVICE_ADDRESS);
    acpi_tables = acpi_table_new(bootinfo);
    if(!acpi_tables)
        KERROR("Error getting ACPI tables");
    pci_enum_ecam(acpi_tables->mcfg);
    
    //End of init
    interrupt_enable();
    kmain(bootinfo);
    BREAKPOINT();
}
