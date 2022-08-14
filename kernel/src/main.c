#include <kernel.h>
#include "../tux.png.h"
uint64_t kmain(struct bootinfo *bootinfo){
	KDEBUG("ARCH "ARCH);
	KDEBUG("FB %dx%d at 0x%x (%d MB)", fb.width, fb.height, fb.buffer, BYTES_TO_MB(fb.size));
	KDEBUG("Available system memory %d MB", BYTES_TO_MB(heap.n_block*HEAP_BLOCK_SIZE));
	struct efi_time t = {0};
	efi_status_t status = bootinfo->SystemTable->RuntimeServices->GetTime(&t, 0x0);
	KDEBUG("GetTime returned 0x%x\n", status);
	framebuffer_clear(
			&fb,
			&(framebuffer_pixel){.Green = 0x00 , .Blue = 0x00 , .Red = 0x00 , .Alpha = 0x0 
	});
	char *test = "HI ! Loonix";
	int pos = 1;
	for(int i = 0; i < strlen(test); i++){
		font_ascii_draw_framebuffer(&fb, 
				pos+(i*FONT_ASCII_WIDTH)+ (fb.width/2) - (FONT_ASCII_HEIGHT/2), 
				(fb.width/2) - (FONT_ASCII_WIDTH/2), 
				test[i]
				);
		pos++;
	}
	graphics_sprite *tux = TUX_SPRITE_NEW();
	TUX_SPRITE_INIT(tux);
	framebuffer_draw_sprite(&fb,
			0, 0,
			tux);
	framebuffer_update_device(&fb);
	graphics_sprite_free(tux);
	kprint("Welcome to l00n1x !\n");	
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
