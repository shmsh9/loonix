#include <kernel.h>


void karray_test(karray *array, uint64_t n){
	for(uint64_t i = 0 ; i < n; i++){
		karray_push(array, i);
	}
		
}
uint64_t kmain(struct bootinfo *bootinfo){
	kheap_init(&heap);
	kheap_add_block(&heap, HEAP_START);
	SERIAL_INIT();
	kprintf("HEAP_START 0x%x\n", HEAP_START);
	kprint("ARCH "ARCH"\n");
	char *heap_motd = strdup("Welcome to l00n1x !\n");
	kprint(heap_motd);	
	kfree(heap_motd);
	karray *a = karray_new(4);
	if(a){
		karray_test(a, 128);
		karray_print(a);
		kheap_debug_print(&heap);
		karray_free(a);	
		kheap_debug_print(&heap);
	}
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
