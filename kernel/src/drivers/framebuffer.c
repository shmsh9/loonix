#include <drivers/framebuffer.h>
uint64_t framebuffer_last_draw_pixel_tick = 0;
void framebuffer_device_draw_pixel_slow(framebuffer_device *framebuffer, int64_t x, int64_t y, framebuffer_pixel *pixel){
    if(!framebuffer)
        return;
    if(!framebuffer->buffer)
        return;
    if(!pixel)
        return;
    //do not draw invisible pixels
    if(!pixel->Alpha)
        return;  
    if(x >= framebuffer->width || y >= framebuffer->height)
        return;
    if(x < 0 || y < 0)
        return;
    uint64_t pos = framebuffer->width*y+x;
    //if(pos > framebuffer->width*framebuffer->height)
    //    return;
    framebuffer->dst[pos] = *pixel;
    framebuffer_last_draw_pixel_tick = cpu_get_tick();
}
void framebuffer_device_draw_pixel_fast(framebuffer_device *framebuffer, uint64_t x, uint64_t y, framebuffer_pixel *pixel){
    if(!pixel->Alpha)
        return;  
    uint64_t pos = framebuffer->width*y+x;
    framebuffer->dst[pos] = *pixel;
    framebuffer_last_draw_pixel_tick = cpu_get_tick();

}
void framebuffer_device_clear(framebuffer_device *framebuffer, framebuffer_pixel *pixel){
    if(!framebuffer->buffer)
        return;
    uint32_t px = *(uint32_t *)pixel;
    /*
    uint64_t pixel_casted64 = (uint64_t)(
        (uint64_t)px << 32 | px
    );
    __uint128_t pixel_casted128 = (__uint128_t)pixel_casted64 << 64 | pixel_casted64;
    */
    memset(framebuffer->dst, (uint8_t)(px & 0xff), framebuffer->size);
    framebuffer_last_draw_pixel_tick = cpu_get_tick();
}

framebuffer_device *framebuffer_device_new(uintptr_t address, uint64_t width, uint64_t height, uint16_t flags){
    if(!address){
        KERROR("0x%x is not a valid framebuffer address !", address);
        return 0x0;
    }
    framebuffer_device *ret = kcalloc(sizeof(framebuffer_device), 1);
    uint64_t size = width*height*sizeof(graphics_pixel);
    *ret = (framebuffer_device){
        .buffer = (framebuffer_pixel *)address,
        .double_buffer = 0x0,
        .width = width,
        .height = height,
        .size = size,
        .flags = flags,
        .dst = 0x0
    };
    if( (flags & FRAMEBUFFER_DOUBLE_BUFFERING) == FRAMEBUFFER_DOUBLE_BUFFERING){
        void *double_buffer = kmalloc(size);
        if(!double_buffer){
            KERROR("not enough memory for double buffering");
        }
        else{
            //KDEBUG("double buffer at 0x%x", double_buffer);
            ret->double_buffer = double_buffer;
        }
    }
    ret->dst = ret->double_buffer == 0x0 ? ret->buffer : ret->double_buffer;
    return ret;
}

void framebuffer_device_free(framebuffer_device *framebuffer){
    if(framebuffer->double_buffer)
        kfree(framebuffer->double_buffer);
    kfree(framebuffer);
}
uint64_t framebuffer_last_device_update_tick = 0;
void framebuffer_device_update(framebuffer_device *framebuffer){
    if(!framebuffer)
        return;
    if(!framebuffer->double_buffer)
        return;
    if(framebuffer_last_device_update_tick <= framebuffer_last_draw_pixel_tick)
        memcpy(framebuffer->buffer, framebuffer->double_buffer, framebuffer->size);
    framebuffer_last_device_update_tick = cpu_get_tick();
}
void framebuffer_device_update_partial(framebuffer_device *framebuffer, uint64_t offset, uint64_t size){
    if(offset+size > framebuffer->size){
        KERROR("offset+size > framebuffer->size");
        return;
    }
    if(!framebuffer)
        return;
    if(!framebuffer->double_buffer)
        return;
    if(framebuffer_last_device_update_tick < framebuffer_last_draw_pixel_tick)
        memcpy(framebuffer->buffer+offset, framebuffer->double_buffer+offset, size);
    framebuffer_last_device_update_tick = cpu_get_tick();
}

void framebuffer_device_draw_sprite_slow(framebuffer_device *framebuffer, int64_t x, int64_t y, graphics_sprite *sprite){
    int64_t current_y = y;
    for(uint64_t current_pixel = 0; current_pixel < sprite->size;){
       for(int64_t current_x = x; current_x < ((int64_t)(sprite->width))+x; current_x++){
            framebuffer_device_draw_pixel_slow(framebuffer, current_x, current_y, &sprite->pixels[current_pixel]);
            current_pixel++;
       }
       current_y++;
    }
}
//can not handle transparency && is unsafe
void framebuffer_device_draw_sprite_fast(framebuffer_device *framebuffer, int64_t x, int64_t y, graphics_sprite *sprite){
    for(uint64_t sprite_line = 0; sprite_line < sprite->height; sprite_line++, y++){
        memcpy(
            framebuffer->dst+(framebuffer->width*y+x),
            sprite->pixels+((sprite->width)*sprite_line),
            (sprite->width)*sizeof(graphics_pixel)
        );
        framebuffer_last_draw_pixel_tick = cpu_get_tick();
    }
}
void framebuffer_device_scroll_down(framebuffer_device *framebuffer, uint64_t y){
    if(!framebuffer){
        return;
	}
   	uint8_t *end_fb = (uint8_t*)framebuffer->dst + framebuffer->width*framebuffer->height*sizeof(graphics_pixel);
	uint64_t row_size = sizeof(graphics_pixel)*framebuffer->width;
	memcpy(
		framebuffer->dst, 
		framebuffer->dst+(y*framebuffer->width), 
		framebuffer->width*framebuffer->height*sizeof(graphics_pixel) - (y*framebuffer->width*sizeof(graphics_pixel))
	);
   	memset(
		end_fb - (row_size*y),
		0x00,
		row_size*y
	);
	framebuffer_last_draw_pixel_tick = cpu_get_tick();
}
