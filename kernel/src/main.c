#include <kernel.h>
#include <kstd/async.h>


#define async_shell_cmd_sz 8
#define async_shell_prmpt "asyncsh> "
char async_shell_char = 0x0;
char async_shell_cmd[async_shell_cmd_sz+1] = {0};
uint8_t async_shell_cmd_i = 0;

ASYNC_FN(a1, bool *b, {
	*b = false;
})
bool asyncb = false;
ASYNC_FN(asynct, void *p, {
	ASYNC_AWAIT_THEN(ASYNC_CALL(a1, &asyncb),{
		KMESSAGE("should be here second");
	});
})
ASYNC_FN(async_shell, char *c, {
	ASYNC_AWAIT_THEN(ASYNC_CALL(kgetchar_async, &async_shell_char), {
		switch(async_shell_char){
			case 0x0d:
			case 0x0a:
				kprintf("\n%s : command not found\n%s", async_shell_cmd, async_shell_prmpt);
				memset(async_shell_cmd, 0, async_shell_cmd_sz);
				async_shell_cmd_i = 0;
				ASYNC_CALL(asynct, 0x0);
				KMESSAGE("should be here first");
				break;
			default:
				if(async_shell_cmd_i+1 < async_shell_cmd_sz){
					kputc(async_shell_char);
					async_shell_cmd[async_shell_cmd_i++] = async_shell_char;
				}
		}
	})
})
int async_shell_task(void *d, task *t){
	kprintf("%s", async_shell_prmpt);
	while(1){
		ASYNC_CALL(async_shell, 0x0);
		ASYNC_RUN();
	}
	return 0;
}
int vt100_update_task(void *d, task *t){
	while(1){
		vt100_console_update_draw_screen(fb);
	}
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
	task_unlock();

	BREAKPOINT();
	KMESSAGE("breakpoint passed X_X");
	return 0xcafe;
}
