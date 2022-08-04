#include <kernel.h>

KHEAPLCAB HEAP;

#ifdef __x86_64__
	//ugly
	#define ARCH "x86_64"
	#define HEAP_START 0x10FFFF
#endif

#ifdef __aarch64__
	#define ARCH "aarch64"
	#define HEAP_START (uint64_t)bootinfo->kernelbase+bootinfo->kernelsize	
#endif

uint64_t kmain(struct bootinfo *bootinfo){
  k_heapLCABInit(&HEAP);
  k_heapLCABAddBlock(&HEAP, HEAP_START, 0x10000);
	SERIAL_INIT();
	kprintf("HEAP_START 0x%x\n", HEAP.fblock);
	kprint("ARCH "ARCH"\n");
	char *heap_motd = strdup("Welcome to l00n1x !\n");
	kprint(heap_motd);	
	kfree(heap_motd);
	karray *a = karray_new(sizeof(uint16_t));

	for(int i = 0 ; i < 128; i++)
		karray_push(a, i);
	/*
	kprint("{ ");
	for(int i = 0; i < a->length; i++){
		kprintf("%x ",((uint64_t *)a->array)[i]);
		if(i+1 < a->length)
			kputc(',');
	}
	kprint("}\n");
	karray_free(a);	
	*/
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
