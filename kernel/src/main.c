#include <kernel.h>

event_loop *main_event_loop = 0x0;
uint64_t kmain(bootinfo *bootinfo){
	KMESSAGE("ARCH %s", ARCH_STRING);
	KMESSAGE("Framebuffer %dx%d at 0x%x (%d MB)", fb->width, fb->height, fb->buffer, BYTES_TO_MB(fb->size));
	KMESSAGE("Available system memory %d MB", BYTES_TO_MB(heap.free_memory));
	kprint("Welcome to l00n1x !\n");
	main_event_loop = event_loop_new();
	event_loop_add(
		main_event_loop,
		(void (*)(void *))vt100_console_update_draw_screen,
		(void *)fb,
		1
	);
	event_loop_add(
		main_event_loop,
		shell_non_blocking,
		0x0,
		1
	);
	/*
	event_loop_add(
		main_event_loop,
		(void (*)(void *))kprint,
		"TEST LOOP\n",
		100000000
	);
	*/
	event_loop_loop(main_event_loop);
	KMESSAGE("main_event_loop ended");
	//shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
