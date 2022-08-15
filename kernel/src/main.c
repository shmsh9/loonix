#include <kernel.h>
uint64_t kmain(struct bootinfo *bootinfo){
	KDEBUG("ARCH "ARCH);
	KDEBUG("FB %dx%d at 0x%x (%d MB)", fb.width, fb.height, fb.buffer, BYTES_TO_MB(fb.size));
	KDEBUG("Available system memory %d MB", BYTES_TO_MB(heap.n_block*HEAP_BLOCK_SIZE));
	framebuffer_clear(
			&fb,
			&(framebuffer_pixel){.Green = 0xff , .Blue = 0x00 , .Red = 0x00 , .Alpha = 0x0 
	});
	char *test = "HI ! Loonix";
	int ltest = strlen(test);
	int pos = 1;
	for(uint64_t i = 0; i < ltest; i++){
		font_ascii_draw_framebuffer(&fb,
				pos+(i*FONT_ASCII_WIDTH)+70,
				70,
				test[i]
				);
		//KDEBUG("ascii_pixels_font[%c]", test[i]);
		//KDEBUG("width == %d", font_pixels_ascii[(uint8_t)test[i]].width);
	}
	//graphics_sprite *tux = TUX_SPRITE_NEW();
	KDEBUG("BEFORE");
	graphics_sprite *ascii = font_5x7_new();
	//#include "../tux.png.h"
	//memcpy(tux, TUX_PIXELS, (50*60)*4);
	KDEBUG("AFTER");
	framebuffer_draw_sprite(&fb,
			100, 300,
			ascii);
	framebuffer_update_device(&fb);
	graphics_sprite_free(ascii);
	kprint("Welcome to l00n1x !\n");	
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
