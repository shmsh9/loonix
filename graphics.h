#ifndef GRAPHICS_H_
#define GRAPHICS_H_
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>

EFI_GRAPHICS_OUTPUT_BLT_PIXEL *newScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINT8 colorfill);
void drawPX(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *px, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop,unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int scale);
void PrintPX(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *px, int size);
void drawOnScreen(EFI_GRAPHICS_OUTPUT_BLT_PIXEL px, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *screen, int x, int y, int w, int h, int scale);
void drawSpriteOnScreen(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *sprite, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *screen, int x, int y, int w, int h, int screenw, int screenh, int scale);

EFI_GRAPHICS_OUTPUT_BLT_PIXEL *newScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINT8 colorfill){
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *screen = NULL;
    gBS->AllocatePool(EfiBootServicesData, sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)*gop->Mode->FrameBufferSize, (void *)screen);
    SetMem(screen, gop->Mode->FrameBufferSize*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), colorfill);
    return screen;
}
void drawPX(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *px, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop,unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int scale){
    int j,k = 0;
    for(int i = 0; i < w*h; i++, k++){
        if( i != 0 && i % w == 0){
            j++;
            k = 0;
        }
        /*
        Print(L"px[%d] == { %d, %d, %d, %d }\n", i,px[i].Blue, px[i].Green, px[i].Red, px[i].Reserved);
        Print(L"i : %d, j : %d, k : %d\n", i, j , k );
        */
        gop->Blt(gop, &px[i], EfiBltVideoFill, 0, 0, k*scale, j*scale, scale, scale, 0);
        //uefi_call_wrapper(gop->Blt, 10, gop, (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)&px[i], EfiBltVideoFill, 0, 0, k*scale, j*scale, 10, 10, 0);
    }
}

void PrintPX(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *px, int size){
    for(int i = 0; i < size; i++){
        Print(L"px[%d] == { %d, %d, %d, %d }\n", i,px[i].Blue, px[i].Green, px[i].Red, px[i].Reserved);
    }

}
void drawScreen(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *screen, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, int w, int h){
    drawPX(screen, gop, 0, 0, w, h, 1);
}
void drawOnScreen(EFI_GRAPHICS_OUTPUT_BLT_PIXEL px, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *screen, int x, int y, int w, int h, int scale){
    int pos = x+y*w;
    if(pos < w*h)
        screen[pos] =  px;
    else
        Print(L"ERR drawOnscren() : pos == %d w*h == %d\n", pos, w*h);
}
void drawSpriteOnScreen(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *sprite, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *screen, int x, int y, int w, int h, int screenw, int screenh, int scale){
    int j = y;
    int k = x;

    for(int i = 0; i < w*h; i++, k++){
        if( i != 0 && i % w == 0){
            j++;
            k = x;
        }
        if(sprite[i].Reserved != 255){
            drawOnScreen(sprite[i], screen, k, j, screenw, screenh, scale);
        }
    }
}
#endif