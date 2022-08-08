#include <kernel.h>

KHEAPLCAB HEAP;
#if UINT32_MAX == UINTPTR_MAX
#define STACK_CHK_GUARD 0xe2dee396
#else
#define STACK_CHK_GUARD 0x595e9fbd94fda766
#endif
 
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

#ifdef __x86_64__
	//ugly
	#define ARCH "x86_64"
	#define HEAP_START 0x10FFFF
#endif

#ifdef __aarch64__
	#define ARCH "aarch64"
	#define HEAP_START (uint64_t)bootinfo->kernelbase+bootinfo->kernelsize	
#endif
void karray_test(karray *array, uint64_t n){
	for(uint64_t i = 0 ; i < n; i++){
		karray_push(array, i);
	}
		
}
uint64_t kmain(struct bootinfo *bootinfo){
  k_heapLCABInit(&HEAP);
  k_heapLCABAddBlock(&HEAP, HEAP_START, 0x10000);
	SERIAL_INIT();
	//kprintf("HEAP_START 0x%x\n", HEAP.fblock);
	//kprint("ARCH "ARCH"\n");
	//char *heap_motd = strdup("Welcome to l00n1x !\n");
	//kprint(heap_motd);	
	//kfree(heap_motd);
	
	karray *a = karray_new(sizeof(uint64_t));
	karray_test(a, 17);
	karray_print(a);
	karray_free(a);	
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
