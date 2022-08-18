#include <kernel.h>
uint64_t kmain(struct bootinfo *bootinfo){
	KDEBUG("ARCH %s", ARCH_STRING);
	KDEBUG("Framebuffer %dx%d at 0x%x (%d MB)", fb.width, fb.height, fb.buffer, BYTES_TO_MB(fb.size));
	KDEBUG("Available system memory %d MB", BYTES_TO_MB(heap.free_memory));
	kprint("Welcome to l00n1x !\n");
	builtins_free(0, NULL);
	klist *k = klist_new(NULL);
	for(uint64_t i = 0; i < 2; i++)
		klist_push(k, i);
	builtins_free(0, NULL);
	klist_debug_print(k);
	klist_free(k);
	builtins_free(0, NULL);
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
