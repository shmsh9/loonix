#include <kernel.h>

#if UINT32_MAX == UINTPTR_MAX
	#define STACK_CHK_GUARD 0xe2dee396
#else
	#define STACK_CHK_GUARD 0x595e9fbd94fda766
#endif
#ifdef __x86_64__
	#define ARCH "x86_64"
	#define HEAP_START 0x10FFFF
#endif
#ifdef __aarch64__
	#define ARCH "aarch64"
	#define HEAP_START (uint64_t)bootinfo->kernelbase+bootinfo->kernelsize	
#endif
 
KHEAPLCAB HEAP;
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void karray_test(karray *array, uint64_t n){
	for(uint64_t i = 0 ; i < n; i++){
		karray_push(array, i);
	}
		
}
uint64_t kmain(struct bootinfo *bootinfo){
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
	kheap heap;
	kheap_init(&heap);
	kheap_add_block(&heap, HEAP_START);
	kheap_get_free_mem(&heap, 512);
	//kheap_get_free_mem(&heap, 512);
	kheap_get_free_mem(&heap, 8);
	kheap_debug_print(&heap);
	kprintf("last bitfield == 0x%x\n", heap.root->header[127]);
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
