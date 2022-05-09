#ifndef GRAPHICS_H_
#define GRAPHICS_H_
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>

typedef struct{
    UINT32 bufferSize;
    UINT16 w;
    UINT16 h;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *screen;
}SCREEN;
typedef struct
{
    UINT16 w;
    UINT16 h;
    UINT32 bufferSize;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *sprite;
}SPRITE;

SCREEN *newScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINT16 w, UINT16 h, UINT8 colorfill);
void PrintPX(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *px, int start,int end);
void drawOnScreen(EFI_GRAPHICS_OUTPUT_BLT_PIXEL px, SCREEN *screen, UINT16 x, UINT16 y, UINT8 scale);
void drawSpriteOnScreen(SPRITE *sprite, SCREEN *screen, UINT16 x, UINT16 y, UINT8 scale);
void refreshPartialScreen(SCREEN *screen, UINT16 w, UINT16 h, UINT16 x, UINT16 y);
void refreshSpritePosition(SPRITE *sprite, SCREEN *screen, UINT16 oldx, UINT16 oldy, UINT16 newx, UINT16 newy, UINT8 colorfill, UINT8 scale);
void clearScreen(SCREEN *screen, UINT8 colorfill);

SCREEN *newScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINT16 w, UINT16 h, UINT8 colorfill){
    SCREEN *ret = NULL; 
    gBS->AllocatePool(EfiBootServicesData, sizeof(SCREEN), (void *)&ret);
    ret->screen = NULL;
    ret->bufferSize = w*h;
    ret->w = w;
    ret->h = h;
    ret->gop = gop;
    gBS->AllocatePool(EfiBootServicesData, sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)*(ret->bufferSize), (void *)ret->screen);
    SetMem(ret->screen, ret->bufferSize*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), colorfill);
    return ret;
}
void clearScreen(SCREEN *screen, UINT8 colorfill){
        SetMem(screen->screen, screen->bufferSize*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), colorfill);
}
void PrintPX(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *px, int start,int end){
    for(int i = start; i < end; i++){
        Print(L"px[%d] == { %d, %d, %d, %d }\n", i,px[i].Blue, px[i].Green, px[i].Red, px[i].Reserved);
    }
}
void drawScreen(SCREEN *screen){
    //drawPX(screen, gop, 0, 0, w, h, 1);
    screen->gop->Blt(screen->gop, screen->screen, EfiBltBufferToVideo, 0, 0, 0, 0, screen->w, screen->h, 0);
}
void refreshPartialScreen(SCREEN *screen, UINT16 w, UINT16 h, UINT16 x, UINT16 y){
    int pos = x+(y*screen->w);
    if(pos < screen->bufferSize){
        screen->gop->Blt(screen->gop, screen->screen, EfiBltBufferToVideo, x, y, x, y, w ,h, 0);
    }
    /*
    else{
        Print(L"ERR drawPartialScreen() : pos == %d w*h == %d\n %dx%d\n", pos, wscreen*hscreen, wscreen,hscreen);
    }
    */
}
inline void drawOnScreen(EFI_GRAPHICS_OUTPUT_BLT_PIXEL px, SCREEN *screen, UINT16 x, UINT16 y, UINT8 scale){
    int pos = x+(y*screen->w);
    if(pos < screen->bufferSize){
        if(px.Reserved != 255)
            screen->screen[pos] = px;
        //Print(L"drawOnScreen() screen[%d]\n", pos);
    }
    else
        Print(L"ERR drawOnscren() : pos == %d w*h == %d\n%dx%d\n", pos, screen->bufferSize, screen->w, screen->h);
}
void drawSpriteOnScreen(SPRITE *sprite, SCREEN *screen, UINT16 x, UINT16 y, UINT8 scale){
    int j = y;
    int k = x;
    for(int i = 0; i < sprite->bufferSize; i++, k++){
        if( i != 0 && i % screen->w == 0){
            j++;
            k = x;
        }
        drawOnScreen(sprite->sprite[i], screen, k, j, scale);
    }
}
void refreshSpritePosition(SPRITE *sprite, SCREEN *screen, UINT16 oldx, UINT16 oldy, UINT16 newx, UINT16 newy, UINT8 colorfill, UINT8 scale){
    //remove old sprite from screen
    int j = oldy;
    int k = oldx;
    for(int i = 0; i < sprite->bufferSize; i++, k++){
        if( i != 0 && i % screen->w == 0){
            j++;
            k = oldx;
        }
        drawOnScreen((EFI_GRAPHICS_OUTPUT_BLT_PIXEL){colorfill,colorfill,colorfill,0}, screen, k, j, scale);
    }
    //draw new sprite
    drawSpriteOnScreen(sprite, screen, newx, newy, scale);
    //refresh screen
    refreshPartialScreen(screen, sprite->w, sprite->h, oldx, oldy);
    refreshPartialScreen(screen, sprite->w, sprite->h, newx, newy);
 
}
#endif