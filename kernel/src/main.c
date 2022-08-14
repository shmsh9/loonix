#include <kernel.h>

uint64_t kmain(struct bootinfo *bootinfo){
	KDEBUG("ARCH "ARCH);
	KDEBUG("FB %dx%d at 0x%x (%d MB)", fb.width, fb.height, fb.buffer, BYTES_TO_MB(fb.size));
	KDEBUG("Available system memory %d MB", BYTES_TO_MB(heap.n_block*HEAP_BLOCK_SIZE));
	struct efi_time t = {0};
	efi_status_t status = bootinfo->SystemTable->RuntimeServices->GetTime(&t, 0x0);
	KDEBUG("GetTime returned 0x%x\n", status);
	graphics_pixel font_pixels_H[] = {
		GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_WHITE,
		GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE,
		GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_WHITE,
		GRAPHICS_PIXEL_WHITE, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_TRANSPARENT, GRAPHICS_PIXEL_WHITE,
	};
	graphics_sprite font_sprite_H = {
		.height = 4,
		.width = 4,
		.pixels = font_pixels_H,
	};
	framebuffer_clear(
			&fb,
			&(framebuffer_pixel){.Green = 0x00 , .Blue = 0x00 , .Red = 0x00 , .Alpha = 0x0 
	});
	framebuffer_draw_sprite(
		&fb, 
		GRAPHICS_CENTER_WIDTH(fb,  font_sprite_H),
		GRAPHICS_CENTER_HEIGHT(fb, font_sprite_H), 
		&font_sprite_H
		);
	framebuffer_update_device(&fb);
	kprint("Welcome to l00n1x !\n");	
	shell();
	while(1){
		/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}
