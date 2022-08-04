#include <kernel.h>

KHEAPLCAB HEAP;

#ifdef __x86_64__
	//ugly
	#define HEAP_START 0x10FFFF
#endif

#ifdef __aarch64__
	#define HEAP_START (uint64_t)bootinfo->kernelbase+bootinfo->kernelsize	
#endif

uint64_t kmain(struct bootinfo *bootinfo){
  k_heapLCABInit(&HEAP);                              /* initialize the heap */
  k_heapLCABAddBlock(&HEAP, HEAP_START, 0x10000);  /* add block to heap */
	SERIAL_INIT();
	kprintf("HEAP_START = 0x%x\n", HEAP.fblock);
	char *f = strdup("Hello World from heap !\n");
  char *f2 = strdup("HEnlo again bugless heap\n");
	kprint(f);
	kprint(f2);
	kfree(f);
	kfree(f2);
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
