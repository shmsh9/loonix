#include <kernel.h>
#include <kstd/async.h>

ASYNC_FN(a1, bool *b, {
	*b = 1;
})
ASYNC_FN(a2, bool *b, {
	*b = 0;
})
bool shared_state = 0;
ASYNC_FN(afoo, bool *b, {
	ASYNC_AWAIT_THEN(ASYNC_CALL(a1, &shared_state), {
		kprintf("afoo(%d) b == %s\n", state.curr_state , shared_state ? "true" : "false");
	});
	ASYNC_AWAIT(ASYNC_CALL(a2, &shared_state));
	kprintf("afoo(%d) b == %s\n", state.curr_state ,shared_state ? "true" : "false");
})
int asynct(void *data, task *t){
	bool b = 0;
	ASYNC_CALL(afoo, &b);
	ASYNC_RUN();
	return 0;
}

uint64_t kmain(bootinfo *bootinfo){
	KMESSAGE("ARCH %s", ARCH_STRING);
	KMESSAGE("Framebuffer %dx%d at 0x%x (%d MB)", (uint64_t)fb->width, (uint64_t)fb->height, fb->buffer, (uint64_t)BYTES_TO_MB(fb->size));
	KMESSAGE("Available system memory %d MB", (uint64_t)BYTES_TO_MB(heap.free_memory));
	kprint("Welcome to l00n1x !\n");
	task_new(
		(int (*)(void *, task *))shell_rs, 
		0x0, 
		"shell",
		task_priority_high
	);
	task_new(
		asynct, 
		0x0, 
		"ascynt",
		task_priority_low
	);

	task_unlock();

	BREAKPOINT();
	KMESSAGE("breakpoint passed X_X");
	return 0xcafe;
}
