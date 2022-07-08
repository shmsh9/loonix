#include <kernel.h>

uint64_t kmain(struct bootinfo *bootinfo){
	SERIAL_INIT();
	kprintf("Loon1x entry : 0x%x\n", bootinfo->kernelentry);
	shell();
	char c = 'a';
	while(1){
		kputc('C');
		c = kgetchar();
		c++;
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
