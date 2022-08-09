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
/*
	k_heapLCABInit(&HEAP);
	k_heapLCABAddBlock(&HEAP, HEAP_START, 0x10000);
	k_heapLCABAddBlock(&HEAP, HEAP_START+0x10000, 0x10000);
	char *heap_motd = strdup("Welcome to l00n1x !\n");
	kprint(heap_motd);	
	kfree(heap_motd);
	karray *a = karray_new(4);
	if(a){
		karray_test(a, 8192);
		karray_print(a);
		karray_free(a);	
	}
	*/

	//void *foo = kmalloc(32);
	//kheap_debug_print(&heap);
	//kfree(foo);
	//kheap_free_mem(&kalloc_list[0]);
	kprintf("")
	kalloc_list[0] = kheap_get_free_mem(&heap, 32);
	kheap_debug_print(&heap);
	kheap_free_mem(&kalloc_list[0]);
	kheap_debug_print(&heap);

	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
