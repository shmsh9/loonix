#include <kernel.h>

uint64_t kmain(struct bootinfo *bootinfo){
	SERIAL_INIT();
	kprintf("Loon1x entry : 0x%x\n", bootinfo->kernelentry);
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
