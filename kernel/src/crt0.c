#include <shell/builtins.h>
#include <newmem.h>
#include <kernel.h>
#include <bootloader.h>
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;
kheap_allocated_block kalloc_list[KALLOC_LIST_MAX] = {0};
kheap heap;
framebuffer_device fb = {0};
uint32_t kalloc_list_last = 0;
efi_runtime_services *runtime_services = 0;
struct efi_time global_efi_time = {0};

void crt0(struct bootinfo *bootinfo){
    runtime_services = bootinfo->RuntimeServices;
    bootinfo->RuntimeServices->GetTime(&global_efi_time, 0);
    SERIAL_INIT();
    kheap_init(&heap);
    if(!bootinfo->mmap){
        KERROR("fatal cannot retrieve memory map at 0x%x !", bootinfo->mmap);
        BREAKPOINT();
    }
    KDEBUG("mmap at 0x%x", bootinfo->mmap);
    KDEBUG("sizeof(mmap) == %d (probably true)", sizeof(struct efi_memory_descriptor));
    uint8_t *startOfMemoryMap = (uint8_t *)bootinfo->mmap;
    uint8_t *endOfMemoryMap = startOfMemoryMap + bootinfo->mmap_size;
    uint8_t *offset = startOfMemoryMap;
    uint32_t counter = 0;
    uint64_t latest_high = 0;
    struct efi_memory_descriptor *highest_mem_block = 0;
    //minimum of of 2mb is required 1MB for double buffer
    uint64_t min_ram = MB_TO_BYTES(2);
    while (offset < endOfMemoryMap){
        struct efi_memory_descriptor *desc = (struct efi_memory_descriptor *)offset;
        if(desc->type == EFI_CONVENTIAL_MEMORY){
            KDEBUG("EFI_CONVENTIAL_MEMORY %d bytes at 0x%x", desc->pages * HEAP_BLOCK_SIZE, desc->physical_start)
            KDEBUG("mem attributes : 0x%x", desc->attributes);
            uint64_t curr_mem = desc->pages * HEAP_BLOCK_SIZE;
            if(curr_mem >= min_ram && curr_mem > latest_high){
                latest_high = curr_mem;
                highest_mem_block = desc;
            }
        }
        offset += sizeof(struct efi_memory_descriptor);

        counter++;
    }
    if(!highest_mem_block){
        KERROR("ram not found : trying something stupid");
        kheap_add_blocks(&heap, (uintptr_t)bootinfo->kernelbase+bootinfo->kernelsize, HEAP_RAM_NOT_FOUND_DEFAULT);
    }
    else{
        KDEBUG("found highest mem block : 0x%x %dMB", highest_mem_block->physical_start, BYTES_TO_MB(latest_high));
        uintptr_t ram_addr = highest_mem_block->physical_start;
        uint64_t ram_pages_n = highest_mem_block->pages;
        if(ram_addr == (uintptr_t)bootinfo->kernelbase){
            KDEBUG("protecting kernel also at 0x%x", bootinfo->kernelbase);
            ram_addr += bootinfo->kernelsize;
            ram_pages_n -= (bootinfo->kernelsize / HEAP_BLOCK_SIZE)+1;
        }
        kheap_add_blocks(&heap, ram_addr, ram_pages_n);
    }
    //!\ contiguous memory is needed
	fb = framebuffer_new_device(
        bootinfo->framebuffer.address, 
        bootinfo->framebuffer.width, 
        bootinfo->framebuffer.height, 
        bootinfo->framebuffer.size, 
        FRAMEBUFFER_DOUBLE_BUFFERING);
    builtins.length = 0;
    SHELL_INIT_BUILTIN(clear, "clear");
    SHELL_INIT_BUILTIN(help, "help");
    kmain(bootinfo);
}
