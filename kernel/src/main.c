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
		(void (*)(void *))framebuffer_device_update,
		(void *)fb
	);
	while(1){
		shell_non_blocking();
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
