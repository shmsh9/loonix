#include <drivers/framebuffer.h>

void framebuffer_draw_pixel(framebuffer_device *framebuffer, uint16_t x, uint16_t y, framebuffer_pixel *pixel){
    if(!framebuffer->buffer){
        KERROR("framebuffer_device has buffer at 0x0 !");
        return;
    }
    uint64_t pos = framebuffer->width*y+x;
    if(pos > framebuffer->width*framebuffer->height){
        KERROR("cannot pos %d buffer is to small : %dx%d", pos, framebuffer->width, framebuffer->height);
        return;
    }
    switch((uintptr_t)framebuffer->double_buffer){
        case 0x0:
            //KDEBUG("drawing pixel at framebuffer->buffer[%d]\n", pos);
            framebuffer->buffer[pos] = *pixel;
            break;
        default:
            //KDEBUG("drawing pixel at framebuffer->double_buffer[%d]\n", pos);
            framebuffer->double_buffer[pos] = *pixel;
            break;
    }
}

void framebuffer_clear(framebuffer_device *framebuffer, framebuffer_pixel *pixel){
    uint64_t pixel_casted = (uint64_t)(*(uint32_t *)pixel) << 32 | (*(uint32_t *)pixel);
    uint64_t size_64bits = framebuffer->size >> 3;
    switch((uintptr_t)framebuffer->double_buffer){
        case 0x0:
            for(uint64_t i = 0; i < size_64bits; i++){
                ((uint64_t *)framebuffer->buffer)[i] = pixel_casted;
            }
            break;
        default:
            for(uint64_t i = 0; i < size_64bits; i++){
                ((uint64_t *)framebuffer->double_buffer)[i] = pixel_casted;
            }
            framebuffer_update_device(framebuffer);
            break;

    }

}

framebuffer_device framebuffer_new_device(uintptr_t address, uint16_t width, uint16_t height, uint64_t size, uint16_t flags){
    uintptr_t bad_addresses[] = {0, 0xffffffffffffffff};

    for(int i = 0; i < sizeof(bad_addresses)/sizeof(bad_addresses[0]); i++){
        if(address == bad_addresses[i]){
            KERROR("0x%x is not a valid framebuffer address !", address);
            return (framebuffer_device){0};
        }
    }
    framebuffer_device ret = (framebuffer_device){
        .buffer = (framebuffer_pixel *)address,
        .double_buffer = 0x0,
        .width = width,
        .height = height,
        .size = size,
        .flags = flags
        };
    if( (flags & FRAMEBUFFER_DOUBLE_BUFFERING) == FRAMEBUFFER_DOUBLE_BUFFERING){
        void *double_buffer = kcalloc(size, 1);
        if(!double_buffer){
            KERROR("not enough memory for double buffering");
        }
        else{
            KDEBUG("double buffer at 0x%x", double_buffer);
            ret.double_buffer = double_buffer;
        }
    }

    return ret;
}

void framebuffer_free_device(framebuffer_device *framebuffer){
    kfree(framebuffer->double_buffer);
    memset(framebuffer, 0, sizeof(framebuffer_device));
}

void framebuffer_update_device(framebuffer_device *framebuffer){
    switch(framebuffer->flags & FRAMEBUFFER_DIRECT_WRITE){
        case FRAMEBUFFER_DIRECT_WRITE:
            return;
        default:
            break;
    }
    switch((uintptr_t)framebuffer->double_buffer){
        case 0x0:
            KERROR("double buffering is not enabled for framebuffer at 0x%x", framebuffer->buffer);
            break;
        default:
            memcpy(framebuffer->buffer, framebuffer->double_buffer, framebuffer->size);
            break;
    }    
}

void framebuffer_draw_sprite(framebuffer_device *framebuffer, graphics_sprite *sprite, uint16_t x, uint16_t y){
    uint64_t sprite_size = x*y;
    for(uint64_t i = 0; i < sprite_size; i++){
        
    }
}
