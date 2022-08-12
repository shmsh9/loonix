#include <shell/builtins.h>
#include <newmem.h>
#include <kernel.h>
#include <bootloader.h>
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;
kheap_allocated_block kalloc_list[KALLOC_LIST_MAX] = {0};
kheap heap;
framebuffer_device fb = {0};
uint32_t kalloc_list_last = 0;

void __init_glob(struct bootinfo *bootinfo){
    SERIAL_INIT();
	kheap_init(&heap);
    uintptr_t ram_address = (uint64_t)bootinfo->kernelbase+bootinfo->kernelsize;
    while((ram_address+HEAP_BLOCK_SIZE) % ALIGN){
        ram_address++;
    }
    //514 * 4096 == 2MB
	for(int i = 0; i < 512; i++){
		kheap_add_block(&heap, ram_address+(sizeof(memblock)*i));
	}
	fb = framebuffer_new_device(
        bootinfo->framebuffer.address, 
        bootinfo->framebuffer.width, 
        bootinfo->framebuffer.height, 
        bootinfo->framebuffer.size, 
        FRAMEBUFFER_DIRECT_WRITE);
    builtins.length = 0;
    SHELL_INIT_BUILTIN(clear, "clear");
    SHELL_INIT_BUILTIN(help, "help");
    kmain(bootinfo);
}
