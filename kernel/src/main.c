#include <kernel.h>


void karray_test(karray *array, uint64_t n){
	for(uint64_t i = 0 ; i < n; i++){
		karray_push(array, i);
	}
		
}
uint64_t kmain(struct bootinfo *bootinfo){
	kheap_init(&heap);
	for(int i = 0; i < 2; i++){
		kheap_add_block(&heap, HEAP_START+(sizeof(memblock)*i));
	}
	SERIAL_INIT();
	KDEBUG("HEAP_START 0x%x\n", HEAP_START);
	KDEBUG("ARCH "ARCH"\n");
	KDEBUG("FB %dx%d at 0x%x\n", bootinfo->framebuffer.width, bootinfo->framebuffer.height, bootinfo->framebuffer.address);
	framebuffer_device fb = framebuffer_new_device(bootinfo->framebuffer.address, bootinfo->framebuffer.width, bootinfo->framebuffer.height, bootinfo->framebuffer.size);
	for(int i = 0; i < fb.height; i++){
		framebuffer_draw_pixel(&fb, i, i, &(framebuffer_pixel){.Green = 0xff, .Blue = 0x00, .Red = 0x00, .Reserved = 0x00});
	}
	framebuffer_update_device(&fb);
	char *heap_motd = strdup("Welcome to l00n1x !\n");
	kprint(heap_motd);	
	kfree(heap_motd);
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
