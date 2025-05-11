#include <kernel.h>


uint64_t kmain(bootinfo *bootinfo){
	KMESSAGE("ARCH %s", ARCH_STRING);
	KMESSAGE("Framebuffer %dx%d at 0x%x (%d MB)", (uint64_t)fb->width, (uint64_t)fb->height, fb->buffer, (uint64_t)BYTES_TO_MB(fb->size));
	KMESSAGE("Available system memory %d MB", (uint64_t)BYTES_TO_MB(heap.free_memory));
	kprint("Welcome to l00n1x !\n");
	task_new(
		(int (*)(void *, task *))shell,
		0x0, 
		"shell",
		task_priority_high
	);
	task_unlock();

	BREAKPOINT();
	KMESSAGE("breakpoint passed X_X");
	return 0xcafe;
}
