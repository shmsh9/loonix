#ifndef LAMEGAME_H_
#define LAMEGAME_H_
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>

#include "builtin.h"
#include "helpers.h"
#include "graphics.h"

EFI_GRAPHICS_OUTPUT_BLT_PIXEL icon[] = {
    {0,0,0, 255},   {255,0,0,0},    {0,0,0, 255},
    {255,0,0,0},    {0,0,0, 255},   {255,0,0,0},
    {0,0,0, 255},   {255,0,0,0},    {0,0,0, 255} 
};

int lamegame(struct fnargs *args){
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    int w = 800;
    int h = 600;
    LibLocateProtocol(&GraphicsOutputProtocol, (void **)&gop);
    //Print(L"FrameBufferSize == %d\n",gop->Mode->FrameBufferSize);
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *screen = newScreen(gop, 230);
    //PrintPX(screen, gop->Mode->FrameBufferSize);
    //drawPX(icon, gop, 0, 0, 3, 6, 10);
    //drawPX(screen, gop, 0, 0, 1, gop->Mode->FrameBufferSize, 1);
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL tmp = {0, 0, 255, 0};
    drawSpriteOnScreen(icon, screen, 0, 0, 3, 3, w, h, 1);
    drawScreen(screen, gop, w, h);
    FreePool(screen);
	return 0;
}

#endif