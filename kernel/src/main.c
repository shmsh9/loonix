#include <kernel.h>

void timer_foobar(void *data){
	KMESSAGE("%s", data);
}
uint64_t kmain(bootinfo *bootinfo){
	KMESSAGE("ARCH %s", ARCH_STRING);
	KMESSAGE("Framebuffer %dx%d at 0x%x (%d MB)", fb->width, fb->height, fb->buffer, BYTES_TO_MB(fb->size));
	KMESSAGE("Available system memory %d MB", BYTES_TO_MB(heap.free_memory));
	kprint("Welcome to l00n1x !\n");
	timer_event_new(
		(void (*)(void *))vt100_console_update_draw_screen,
		(void *)fb,
		10
	);
	timer_event_new(
		(void (*)(void *))process_scheduler,
		0x0,
		0
	);
	while(1){
		shell_non_blocking();
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
