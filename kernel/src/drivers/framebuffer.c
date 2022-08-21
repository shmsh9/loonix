#include <drivers/framebuffer.h>

inline void framebuffer_device_draw_pixel(framebuffer_device *framebuffer, uint64_t x, uint64_t y, framebuffer_pixel *pixel){
    switch((uintptr_t)framebuffer->buffer){
    case 0x0:
        KERROR("framebuffer_device has buffer at 0x0 !");
        return;
    default:
        break;
    }
    switch((uintptr_t)pixel){
        case 0x0: 
            KERROR("pixel == NULL");
            return;
        default:
            //no not draw invisible pixels
            switch(pixel->Alpha){
                case 0x00:
                    return;
                default:
                    break;
            }
            break;
    }
    uint64_t pos = framebuffer->width*y+x;
    if(pos > framebuffer->width*framebuffer->height){
        KPANIC("cannot pos %d buffer is to small : %dx%d", pos, framebuffer->width, framebuffer->height);
        return;
    }
    graphics_pixel *dst = framebuffer->double_buffer == 0x0 ? framebuffer->buffer : framebuffer->double_buffer;
    dst[pos] = *pixel;
}

void framebuffer_device_clear(framebuffer_device *framebuffer, framebuffer_pixel *pixel){
    if(!framebuffer->buffer)
        return;
    graphics_pixel *dst = framebuffer->double_buffer == 0x0 ? framebuffer->buffer : framebuffer->double_buffer;
    uint32_t px = *(uint32_t *)pixel;
    uint64_t pixel_casted64 = (uint64_t)(
        (uint64_t)px << 32 | px
    );
    //__uint128_t pixel_casted128 = (__uint128_t)pixel_casted64 << 64 | pixel_casted64;
    switch (framebuffer->size % 16){
    case 0:
        __memset_64b(dst, pixel_casted64, framebuffer->size);
        break;
    default:
        memset(dst, (uint8_t)(px & 0xff), framebuffer->size);        
        break;    
    }
}

framebuffer_device *framebuffer_device_new(uintptr_t address, uint64_t width, uint64_t height, uint16_t flags){
    uintptr_t bad_addresses[] = {0, 0xffffffffffffffff};
    for(int i = 0; i < sizeof(bad_addresses)/sizeof(bad_addresses[0]); i++){
        if(address == bad_addresses[i]){
            KERROR("0x%x is not a valid framebuffer address !", address);
            return 0x0;
        }
    }
    framebuffer_device *ret = kcalloc(sizeof(framebuffer_device), 1);
    uint64_t size = width*height*sizeof(graphics_pixel);
    *ret = (framebuffer_device){
        .buffer = (framebuffer_pixel *)address,
        .double_buffer = 0x0,
        .width = width,
        .height = height,
        .size = size,
        .flags = flags
    };
    if( (flags & FRAMEBUFFER_DOUBLE_BUFFERING) == FRAMEBUFFER_DOUBLE_BUFFERING){
        void *double_buffer = kmalloc(size);
        if(!double_buffer){
            KERROR("not enough memory for double buffering");
        }
        else{
            KDEBUG("double buffer at 0x%x", double_buffer);
            ret->double_buffer = double_buffer;
        }
    }

    return ret;
}

void framebuffer_device_free(framebuffer_device *framebuffer){
    if(framebuffer->double_buffer)
        kfree(framebuffer->double_buffer);
    kfree(framebuffer);
}

void framebuffer_device_update(framebuffer_device *framebuffer){
    if(!framebuffer)
        return;
    if(!framebuffer->double_buffer)
        return;
    memcpy(framebuffer->buffer, framebuffer->double_buffer, framebuffer->size);
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
    memcpy(framebuffer->buffer+offset, framebuffer->double_buffer+offset, size);
}

void framebuffer_device_draw_sprite_slow(framebuffer_device *framebuffer, uint64_t x, uint64_t y, graphics_sprite *sprite){
    uint64_t sprite_size = sprite->height * sprite->width;
    uint64_t current_y = y;
    for(uint64_t current_pixel = 0; current_pixel < sprite_size;){
       for(uint64_t current_x = x; current_x < sprite->width+x; current_x++){
           framebuffer_device_draw_pixel(framebuffer, current_x, current_y, &sprite->pixels[current_pixel]);
           current_pixel++;
       }
       current_y++;
    }
}
//can not handle transparency
void framebuffer_device_draw_sprite_fast(framebuffer_device *framebuffer, uint64_t x, uint64_t y, graphics_sprite *sprite){
    /*
    if(sprite->width+x > framebuffer->width){
        framebuffer_device_draw_sprite_slow(framebuffer, x, y, sprite);
        return;
    }
    if(sprite->height+y > framebuffer->height){
        framebuffer_device_draw_sprite_slow(framebuffer, x, y, sprite);
        return;
    }
    */
    graphics_pixel *dst = framebuffer->double_buffer == 0 ? framebuffer->buffer : framebuffer->double_buffer;
    for(uint64_t sprite_line = 0; sprite_line < sprite->height; sprite_line++){
        memcpy(
            dst+(framebuffer->width*y+x),
            sprite->pixels+(sprite->width*sprite_line),
            sprite->width*sizeof(graphics_pixel)
        );
        y++;
    }
}
void framebuffer_device_scroll_down(framebuffer_device *framebuffer, uint64_t y){
    if(!framebuffer)
        return;
    graphics_pixel *dst = framebuffer->double_buffer == 0 ? framebuffer->buffer : framebuffer->double_buffer;
    //uint64_t pixels_to_scroll = framebuffer->width * y;
    for(uint8_t j = 0; j < y; j++){
        for(uint64_t i = 0; i < (framebuffer->width*framebuffer->height)-framebuffer->width; i+= framebuffer->width){
            void *previous_line = (void *)((uintptr_t)dst+(i*sizeof(graphics_pixel)));
            void *next_line = (void *)((uintptr_t)dst+((i+framebuffer->width)*sizeof(graphics_pixel)));
            memcpy(previous_line, next_line, framebuffer->width*sizeof(graphics_pixel));
        }
    }
}
