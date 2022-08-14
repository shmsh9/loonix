#include <kernel.h>

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
	framebuffer_draw_sprite(
		&fb, 
		1,
		1,
		&font_pixels_ascii['H']
		);
	framebuffer_draw_sprite(
		&fb, 
		6,
		1,
		&font_pixels_ascii['I']
		);
	framebuffer_draw_sprite(
		&fb, 
		11,
		1,
		&font_pixels_ascii['@']
		);
	framebuffer_update_device(&fb);
	kprint("Welcome to l00n1x !\n");	
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
