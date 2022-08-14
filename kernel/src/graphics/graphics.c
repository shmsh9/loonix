#include <graphics/graphics.h>

graphics_sprite *graphics_sprite_new(uint64_t x, uint64_t y){
   graphics_sprite *ret = kcalloc(sizeof(graphics_sprite),1);
   if(!ret){
    KERROR("ret == NULL");
    return 0x0;
   }
   ret->pixels = kcalloc(sizeof(graphics_pixel), x*y);
   if(!ret->pixels){
    KERROR("ret->pixels == NULL");
    kfree(ret);
    return 0;
   }
   ret->width = x;
   ret->height = y;
   KDEBUG("returning %dx%d sprite at 0x%d",x, y, ret);
   KDEBUG("at 0x%x", ret);
   return ret;
}

void graphics_sprite_free(graphics_sprite *ptr){
    kfree(ptr->pixels);
    kfree(ptr);
}

