#include <kernel.h>


uint64_t kmain(bootinfo *bootinfo){
	KMESSAGE("ARCH %s", ARCH_STRING);
	KMESSAGE("Framebuffer %dx%d at 0x%x (%d MB)", fb->width, fb->height, fb->buffer, BYTES_TO_MB(fb->size));
	KMESSAGE("Available system memory %d MB", BYTES_TO_MB(heap.free_memory));
	kprint("Welcome to l00n1x !\n");
	task_new((int (*)(void *, task *))shell, 0x0, "shell");
	BREAKPOINT();
	return 0xcafe;
}
