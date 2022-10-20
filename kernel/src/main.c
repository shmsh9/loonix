#include <kernel.h>

void TASK_FUNCTION dumb(void *data, task *t){
	while(1){
		KMESSAGE("up since %d seconds", getuptime100s());
		sleep(1);
	}
	TASK_FUNCTION_END(t);
}
uint64_t kmain(bootinfo *bootinfo){
	KMESSAGE("ARCH %s", ARCH_STRING);
	KMESSAGE("Framebuffer %dx%d at 0x%x (%d MB)", fb->width, fb->height, fb->buffer, BYTES_TO_MB(fb->size));
	KMESSAGE("Available system memory %d MB", BYTES_TO_MB(heap.free_memory));
	kprint("Welcome to l00n1x !\n");
	//task_new((void (*)(void *, task *))shell, 0x0);
	task_new(dumb, 0x0);
	while(1){}
	return 0xcafe;
}
