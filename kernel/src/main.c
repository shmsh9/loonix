#include <kernel.h>

uint64_t kmain(struct bootinfo *bootinfo){
	int r = init_serial();
	if(r == 1){
		return 0xdead;
	}
	kprintf("kbase    : 0x%x\nksize    : 0x%x\n", bootinfo->kernelbase, bootinfo->kernelsize);
	while(1){
	/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}

