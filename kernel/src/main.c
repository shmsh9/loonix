#include <kernel.h>

uint64_t kmain(struct bootinfo *bootinfo){
	SERIAL_PUTCHAR('H');
	SERIAL_PUTCHAR('E');
	SERIAL_PUTCHAR('L');
	SERIAL_PUTCHAR('L');
	SERIAL_PUTCHAR('L');
	SERIAL_PUTCHAR('O');
	SERIAL_PUTCHAR(' ');
	SERIAL_PUTCHAR('W');
	SERIAL_PUTCHAR('O');
	SERIAL_PUTCHAR('R');
	SERIAL_PUTCHAR('L');
	SERIAL_PUTCHAR('D');
	SERIAL_PUTCHAR('!');
	SERIAL_PUTCHAR('\n');
	//Stack problems on aarch64
	kprint("Hello from l00n1x\n");
	kprint("Second kprint()\n");
	kprintf("kbase    : 0x%x\nksize    : 0x%x\n", bootinfo->kernelbase, bootinfo->kernelsize);
	while(1){
	/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}

