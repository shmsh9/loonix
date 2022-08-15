#include <kernel.h>
uint64_t kmain(struct bootinfo *bootinfo){
	KDEBUG("ARCH "ARCH);
	KDEBUG("FB %dx%d at 0x%x (%d MB)", fb.width, fb.height, fb.buffer, BYTES_TO_MB(fb.size));
	KDEBUG("Available system memory %d MB", BYTES_TO_MB(heap.n_block*HEAP_BLOCK_SIZE));
	framebuffer_clear(
			&fb,
			&(framebuffer_pixel){.Green = 0x00 , .Blue = 0x00 , .Red = 0x00 , .Alpha = 0x0 
	});
	char *test = "HI ! Loonix";
	int ltest = strlen(test);
	int pos = 1;
	for(uint64_t i = 0; i < ltest; i++){
		font8x8_draw_framebuffer(&fb,
				pos+(i*8)+70,
				70,
				test[i]
		);
	}
	framebuffer_update_device(&fb);
	kprint("Welcome to l00n1x !\n");	
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
