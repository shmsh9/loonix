#include <kernel.h>

void TASK_FUNCTION timer_task(void *data, task *t){
	while(1){
		KMESSAGE("up since %d seconds", getuptime100s()/100);
		sleep(1);
	}
	TASK_FUNCTION_END(t);
}
void TASK_FUNCTION dumb2(void *data, task *t){
	KMESSAGE("dumb2 print");
	TASK_FUNCTION_END(t);
}

uint64_t kmain(bootinfo *bootinfo){
	KMESSAGE("ARCH %s", ARCH_STRING);
	KMESSAGE("Framebuffer %dx%d at 0x%x (%d MB)", fb->width, fb->height, fb->buffer, BYTES_TO_MB(fb->size));
	KMESSAGE("Available system memory %d MB", BYTES_TO_MB(heap.free_memory));
	kprint("Welcome to l00n1x !\n");
	task_new(timer_task, 0x0);
	task_new((void (*)(void *, task *))shell, 0x0);
	/*
	//task_new(dumb2, 0x0);
	while(1){
		shell_non_blocking();
	}
	*/
	BREAKPOINT();
	return 0xcafe;
}
