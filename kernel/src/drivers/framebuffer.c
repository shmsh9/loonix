#include <drivers/framebuffer.h>

void framebuffer_draw_pixel(framebuffer_device *framebuffer, uint16_t x, uint16_t y, framebuffer_pixel *pixel){
    if(!framebuffer->buffer){
        KERROR("framebuffer_device has buffer at 0x0 !\n");
        return;
    }
    uint64_t pos = framebuffer->width*y+x;
    if(pos > framebuffer->width*framebuffer->height){
        KERROR("cannot pos %d buffer is to small : %dx%d\n", pos, framebuffer->width, framebuffer->height);
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

framebuffer_device framebuffer_new_device(uintptr_t address, uint16_t width, uint16_t height, uint64_t size){
    uintptr_t bad_addresses[] = {0, 0xffffffffffffffff};

    for(int i = 0; i < sizeof(bad_addresses)/sizeof(bad_addresses[0]); i++){
        if(address == bad_addresses[i]){
            KERROR("0x%x is not a valid framebuffer address !\n", address);
            return (framebuffer_device){0};
        }
    }
    void *tmp = kcalloc(size, 1);
    if(!tmp){
        KERROR("not enough memory for double buffering\n");
    }
    return (framebuffer_device){
        .buffer = (framebuffer_pixel *)address,
        .double_buffer = tmp,
        .width = width,
        .height = height,
        .size = size
        };
}

void framebuffer_free_device(framebuffer_device *framebuffer){
    kfree(framebuffer->double_buffer);
    memset(framebuffer, 0, sizeof(framebuffer_device));
}

void framebuffer_update_device(framebuffer_device *framebuffer){
    switch((uintptr_t)framebuffer->double_buffer){
        case 0x0:
            KERROR("double buffering is not enabled for framebuffer at 0x%x\n", framebuffer->buffer);
            break;
        default:
            memcpy(framebuffer->buffer, framebuffer->double_buffer, framebuffer->size);
            break;
    }    
}
