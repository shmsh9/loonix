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

void __init_glob(struct bootinfo *bootinfo){
    runtime_services = bootinfo->RuntimeServices;
    bootinfo->RuntimeServices->GetTime(&global_efi_time, 0);
    SERIAL_INIT();
	kheap_init(&heap);
    for(int i = 0; i < bootinfo->mmap_size; i++){
	    KDEBUG("available pages : %d", bootinfo->mmap[i].pages);
        KDEBUG("pages physical start : 0x%x", bootinfo->mmap[i].physical_start);
        KDEBUG("pages virtual start : 0x%x", bootinfo->mmap[i].virtual_start);
        KDEBUG("mem type 0x%x", bootinfo->mmap[i].type);
        kputc('\n');
    }
    uintptr_t ram_address = (uint64_t)bootinfo->kernelbase+bootinfo->kernelsize;
    
    //!\ contiguous memory is needed
    kheap_add_blocks(&heap, ram_address); 
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
