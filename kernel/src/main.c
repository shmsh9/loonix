#include <kernel.h>

uint64_t kmain(struct bootinfo *bootinfo){
	SERIAL_INIT();
	kprintf("L00n1x started at 0x%x\n", bootinfo->kernelbase);
	kprintf("SERIAL_ADDRESS + 0x5 == 0x%x\n", SERIAL_ADDRESS+0x5);
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}

