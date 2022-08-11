#include <kernel.h>

uint64_t kmain(struct bootinfo *bootinfo){
	KDEBUG("ARCH "ARCH);
	KDEBUG("FB %dx%d at 0x%x (%d Bytes)", fb.width, fb.height, fb.buffer, fb.size);
	karray *arr = karray_new(sizeof(uint16_t));
	for(int i = 0; i < 512; i++)
		karray_push(arr, i);
	karray_print(arr);
	karray_free(arr);
	while (1)
	{
		for(int i = 0; i < 256; i++){
			framebuffer_clear(&fb, &(framebuffer_pixel){.Green = i, .Blue = i, .Red = i, .Alpha = 0x00});
			framebuffer_update_device(&fb);
		}
	}
	
	kprint("Welcome to l00n1x !\n");	
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
