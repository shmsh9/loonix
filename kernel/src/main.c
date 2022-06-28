#include <kernel.h>

uint64_t kmain(struct bootinfo *bootinfo){
	kprintf("L00n1x started at 0x%x\n", bootinfo->kernelbase);
	kprint("foobar\n");
	char c = SERIAL_READCHAR();
	SERIAL_PUTCHAR(c);
    SERIAL_PUTCHAR('B');
    SERIAL_PUTCHAR('o');
    SERIAL_PUTCHAR('B');

	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}

