#include <shell/builtins.h>
#include <newmem.h>
#include <kernel.h>
#include <bootloader.h>
#include <graphics/font.h>

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;
kheap_allocated_block kalloc_list[KALLOC_LIST_MAX] = {0};
kheap heap;
framebuffer_device fb = {0};
uint32_t kalloc_list_last = 0;
efi_runtime_services *runtime_services = 0;
struct efi_time global_efi_time = {0};

__attribute__ ((constructor)) void crt0(struct bootinfo *bootinfo){
    runtime_services = bootinfo->RuntimeServices;
    runtime_services->GetTime(&global_efi_time, 0);
    SERIAL_INIT();
    if(bootinfo->uefi_exit_code)
        KPANIC("uefi_exit_code returned 0x%x", bootinfo->uefi_exit_code);
    if(!bootinfo->mmap)
        KPANIC("cannot retrieve memory map at 0x%x !", bootinfo->mmap);
    
    mmap mmap = mmap_new(bootinfo);
    //mmap_debug_print(&mmap);
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
    if(ram_addr == (uintptr_t)bootinfo->kernelbase){
        KDEBUG("protecting kernel also at 0x%x", bootinfo->kernelbase);
        ram_addr += bootinfo->kernelsize+HACK_UGLY_OFFSET;
        //ram_pages_n -= (bootinfo->kernelsize+HACK_UGLY_OFFSET / HEAP_BLOCK_SIZE)+1;
    }
    kheap_init(&heap);
    kheap_add_blocks(&heap, ram_addr, ram_pages_n);
   
    //!\ contiguous memory is needed
    fb = framebuffer_new_device(
        bootinfo->framebuffer.address, 
        bootinfo->framebuffer.width, 
        bootinfo->framebuffer.height, 
        bootinfo->framebuffer.size, 
        FRAMEBUFFER_DOUBLE_BUFFERING);
    font_ascii_init(); 
    builtins.length = 0;
    SHELL_INIT_BUILTIN(clear, "clear");
    SHELL_INIT_BUILTIN(help, "help");
    kmain(bootinfo);
}
