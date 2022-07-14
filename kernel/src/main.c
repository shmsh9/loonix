#include <kernel.h>

uint64_t kmain(struct bootinfo *bootinfo){
	SERIAL_INIT();
	kprintf("__HEAP_START = 0x%x\n", __HEAP_START);
	char *f = kmalloc(20);
	f[19] = 0x0;
	memcpy(f, "welcome to l00n1x\n", 19);
	kprint(f);
	kfree(f);
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
