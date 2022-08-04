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
  k_heapLCABInit(&HEAP);
  k_heapLCABAddBlock(&HEAP, HEAP_START, 0x10000);
	SERIAL_INIT();
	kprintf("HEAP_START at 0x%x\n", HEAP.fblock);
	char *heap_motd = strdup("Welcome to l00n1x !\n");
	kprint(heap_motd);	
	kfree(heap_motd);	
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
