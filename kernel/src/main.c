#include <kernel.h>
uint64_t kmain(struct bootinfo *bootinfo){
	KDEBUG("ARCH %s", ARCH_STRING);
	KDEBUG("Framebuffer %dx%d at 0x%x (%d MB)", fb.width, fb.height, fb.buffer, BYTES_TO_MB(fb.size));
	KDEBUG("Available system memory %d MB", BYTES_TO_MB(heap.free_memory));
	kprint("Welcome to l00n1x !\n");
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
