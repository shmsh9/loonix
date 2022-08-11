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
	for(int i = 0; i < bootinfo->framebuffer.size; i += 4){
		uint8_t pix[4] = {i, i , i , i};
		memcpy( (void *)(bootinfo->framebuffer.address+i), &pix, 4);
	}
	//KDEBUG("sizeof(memblock) == %d\n", sizeof(memblock));
	char *heap_motd = strdup("Welcome to l00n1x !\n");
	kprint(heap_motd);	
	kfree(heap_motd);
	karray *a = karray_new(sizeof(uint64_t));
	if(a){
		karray_test(a, 512);
		//kheap_debug_print(&heap);
		//karray_print(a);
		karray_free(a);	
	}
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
