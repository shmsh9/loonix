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
    KDEBUG("mmap at 0x%x", bootinfo->mmap);
    KDEBUG("sizeof(mmap) == %d (probably true)", sizeof(struct efi_memory_descriptor));
    
    uint8_t *startOfMemoryMap = (uint8_t *)bootinfo->mmap;
    uint8_t *endOfMemoryMap = startOfMemoryMap + bootinfo->mmap_size;
    uint8_t *offset = startOfMemoryMap;
    uint32_t counter = 0;
    if(!bootinfo->mmap){
        KERROR("fatal cannot retrieve memory map at 0x%x !");
        BREAKPOINT();
    }
    while (offset < endOfMemoryMap){
        struct efi_memory_descriptor *desc = (struct efi_memory_descriptor *)offset;
        if(desc->type == EFI_CONVENTIAL_MEMORY){
            KDEBUG("EFI_CONVENTIAL_MEMORY %d bytes at 0x%x", desc->pages * HEAP_BLOCK_SIZE, desc->physical_start)
            if(desc->pages >= 0x35e1){
                kheap_add_blocks(&heap, desc->physical_start, desc->pages);
            }
        }
        offset += sizeof(struct efi_memory_descriptor);

        counter++;
    }
    if(!heap.n_block){
        KERROR("ram not found : trying something stupid");
        kheap_add_blocks(&heap, (uintptr_t)bootinfo->kernelbase+bootinfo->kernelsize, HEAP_RAM_NOT_FOUND_DEFAULT);
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
