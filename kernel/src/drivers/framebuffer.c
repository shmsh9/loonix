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

framebuffer_device framebuffer_new_device(uintptr_t address, uint16_t width, uint16_t height, uint64_t size){
    if(!address || address == -1){
        KERROR("0x%x is not a valid framebuffer address !\n", address);
        return (framebuffer_device){0};
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
