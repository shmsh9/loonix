#include <kernel.h>
#include <drivers/serial.h>

uint64_t kmain(struct bootinfo *bootinfo){
	kprint("Hello from l00n1x\n");
	kprint("Second kprint()\n");
	kprintf("kbase    : 0x%x\nksize    : 0x%x\n", bootinfo->kernelbase, bootinfo->kernelsize);
	while(1){
	/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}

