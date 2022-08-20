#include <drivers/framebuffer.h>

inline void framebuffer_draw_pixel(framebuffer_device *framebuffer, uint64_t x, uint64_t y, framebuffer_pixel *pixel){
    switch((uintptr_t)framebuffer->buffer){
    case 0x0:
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
                    switch((uintptr_t)framebuffer->buffer){
                        case 0x0:
                            KERROR("framebuffer_device has buffer at 0x0 !");
                            return;
                        default:
                            break;
                    }
                    break;
            }
            break;
    }
    uint64_t pos = framebuffer->width*y+x;
    if(pos > framebuffer->width*framebuffer->height){
        KPANIC("cannot pos %d buffer is to small : %dx%d", pos, framebuffer->width, framebuffer->height);
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

framebuffer_device framebuffer_new_device(uintptr_t address, uint64_t width, uint64_t height, uint64_t size, uint16_t flags){
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
        void *double_buffer = kmalloc(size);
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
    switch((uintptr_t)framebuffer->buffer){
    case 0x0:
        return;
    default:
        break;
    }
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
void framebuffer_update_device_partial(framebuffer_device *framebuffer, uint64_t offset, uint64_t size){
    if(offset+size > framebuffer->size){
        KERROR("offset+size > framebuffer->size");
        return;
    }
    switch((uintptr_t)framebuffer->buffer){
    case 0x0:
        return;
    default:
        break;
    }
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
            memcpy(framebuffer->buffer+offset, framebuffer->double_buffer+offset, size);
            break;
    }    
}

void framebuffer_draw_sprite(framebuffer_device *framebuffer, uint64_t x, uint64_t y, graphics_sprite *sprite){
    uint64_t sprite_size = sprite->height * sprite->width;
    //KDEBUG("sprite_size == %d", sprite_size);
   //KDEBUG("drawing sprite x : %d y : %d", x, y);
    //KDEBUG("sprite->height == %d", sprite->height);
    //KDEBUG("sprite->width == %d", sprite->width);
    uint64_t current_y = y;
    for(uint64_t current_pixel = 0; current_pixel < sprite_size;){
       for(uint64_t current_x = x; current_x < sprite->width+x; current_x++){
           framebuffer_draw_pixel(framebuffer, current_x, current_y, &sprite->pixels[current_pixel]);
           current_pixel++;
       }
       current_y++;
    }
}
void framebuffer_draw_sprite_fast(framebuffer_device *framebuffer, uint64_t x, uint64_t y, graphics_sprite *sprite){
    graphics_pixel *dst = framebuffer->double_buffer == 0 ? framebuffer->buffer : framebuffer->double_buffer;
    for(uint64_t sprite_line = 0; sprite_line < sprite->height; sprite_line++){
        memcpy(
            dst+(framebuffer->width*y+x),
            sprite->pixels+(sprite->width*y),
            sprite->width*sizeof(graphics_pixel)
        );
        y++;
    }
}
void framebuffer_scroll_down(framebuffer_device *framebuffer, uint64_t y){
    switch ((uintptr_t)framebuffer){
    case 0x0:
        KDEBUG("framebuffer == 0x%x", framebuffer);
        return;
    
    default:
        switch ((uintptr_t)framebuffer->buffer){
        case 0x0:
            KDEBUG("framebuffer->buffer == 0x%x", framebuffer->buffer);
            return;
            break;
        default:
            switch ((uintptr_t)framebuffer->double_buffer){
            case 0x0:
                for(uint8_t j = 0; j < y; j++){
                    for(uint64_t i = 0; i < (framebuffer->width*framebuffer->height)-framebuffer->width; i+= framebuffer->width){
                        void *previous_line = (void *)((uintptr_t)framebuffer->buffer+(i*sizeof(graphics_pixel)));
                        void *next_line = (void *)((uintptr_t)framebuffer->buffer+((i+framebuffer->width)*sizeof(graphics_pixel)));
                        memcpy(previous_line, next_line, framebuffer->width*sizeof(graphics_pixel));
                    }
                }
                break;
            
            default:
                for(uint8_t j = 0; j < y; j++){
                    for(uint64_t i = 0; i < (framebuffer->width*framebuffer->height)-framebuffer->width; i+= framebuffer->width){
                        void *previous_line = (void *)((uintptr_t)framebuffer->double_buffer+(i*sizeof(graphics_pixel)));
                        void *next_line = (void *)((uintptr_t)framebuffer->double_buffer+((i+framebuffer->width)*sizeof(graphics_pixel)));
                        memcpy(previous_line, next_line, framebuffer->width*sizeof(graphics_pixel));
                    }
                }
                break;
            }
            break;
        }
    }
}
