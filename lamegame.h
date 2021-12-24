#ifndef LAMEGAME_H_
#define LAMEGAME_H_
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>

#include "builtin.h"
#include "helpers.h"
#include "graphics.h"

EFI_GRAPHICS_OUTPUT_BLT_PIXEL icon[] = {
    {3,57,127,0},     {0,40,255,0},     {255,255,255,0},  
    {3,57,127,0},     {255,255,255,0},  {255,255,255,0},  
    {3,57,127,0},     {255,255,255,0},  {255,255,255,0},
    {3,57,127,0},     {255,255,255,0},  {0,40,255,0},  
    {3,57,127,0},     {255,255,255,0},  {0,40,255,0},  
    {3,57,127,0},     {255,255,255,0},  {0,40,255,0}


};
int lamegame(struct fnargs *args){
	EFI_STATUS rc;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    rc = LibLocateProtocol(&GraphicsOutputProtocol, (void **)&gop);
    Print(L"FrameBufferSize == %d\n",gop->Mode->FrameBufferSize);
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *screen = newScreen(gop, 230);
    //PrintPX(screen, gop->Mode->FrameBufferSize);
    //drawPX(icon, gop, 0, 0, 3, 6, 10);
    //drawPX(screen, gop, 0, 0, 1, gop->Mode->FrameBufferSize, 1);
    drawScreen(screen, gop, 800, 600);
    while(1)
    FreePool(screen);
	return 0;
}

#endif