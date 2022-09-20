#include <kernel.h>

uint64_t kmain(bootinfo *bootinfo){
	KMESSAGE("ARCH %s", ARCH_STRING);
	KMESSAGE("Framebuffer %dx%d at 0x%x (%d MB)", fb->width, fb->height, fb->buffer, BYTES_TO_MB(fb->size));
	KMESSAGE("Available system memory %d MB", BYTES_TO_MB(heap.free_memory));
	kprint("Welcome to l00n1x !\n");
	event_loop_init();
	event_loop_add(
		(void (*)(void *))vt100_console_update_draw_screen,
		(void *)fb,
		100000
	);
	event_loop_add(
		shell_non_blocking,
		0x0,
		10000
	);
	//shell();
	event_loop_loop();
	KMESSAGE("event_loop_loop() ended");
	//shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
