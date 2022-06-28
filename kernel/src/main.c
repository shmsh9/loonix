#include <kernel.h>

uint64_t kmain(struct bootinfo *bootinfo){
	SERIAL_INIT();
	kprintf("L00n1x started at 0x%x\n", bootinfo->kernelbase);
	kprint("foobar\n");
	char c = SERIAL_READCHAR();
	SERIAL_PUTCHAR(c);

	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}

