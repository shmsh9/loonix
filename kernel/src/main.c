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
	kprintf("HEAP_START 0x%x\n", HEAP_START);
	kprint("ARCH "ARCH"\n");
	kprintf("sizeof(memblock) == %d\n", sizeof(memblock));
	char *heap_motd = strdup("Welcome to l00n1x !\n");
	kprint(heap_motd);	
	kfree(heap_motd);
	karray *a = karray_new(sizeof(uint8_t));
	if(a){
		karray_test(a, 512);
		kheap_debug_print(&heap);
		karray_print(a);
		karray_free(a);	
	}
	kfree((void *)0x39201);
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
