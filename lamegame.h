#ifndef LAMEGAME_H_
#define LAMEGAME_H_
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>

#include "builtin.h"
#include "helpers.h"
#include "graphics.h"

EFI_GRAPHICS_OUTPUT_BLT_PIXEL icon[] = {
    {0,0,0, 255},   {0,0,255,0},    {0,0,0, 255},
    {0,0,255,0},    {0,0,0, 255},   {0,0,255,0},
    {0,0,0, 255},   {0,0,255,0},    {0,0,0, 255},
    {255,0,0,0},    {255,0,0,0},    {255,0,0,0}

};

int lamegame(struct fnargs *args){
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    UINTN sizeofinfo;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info = NULL;
    SPRITE sprite = {3, 4, 3*4, icon};
    LibLocateProtocol(&GraphicsOutputProtocol, (void **)&gop);
    gop->QueryMode(gop, gop->Mode->Mode, &sizeofinfo, &info);
    int w = info->HorizontalResolution;
    int h = info->VerticalResolution;
    SCREEN *screen = newScreen(gop, w, h, 255);
    Print(L"%dx%d\n", info->HorizontalResolution, info->VerticalResolution);
    EFI_INPUT_KEY k = {0, 0};
    UINT32 x = 0;
    UINT32 y = 0;
    args->SystemTable->ConOut->EnableCursor(args->SystemTable->ConOut, FALSE);
	args->SystemTable->ConOut->ClearScreen(args->SystemTable->ConOut);
    drawScreen(screen);
    drawSpriteOnScreen(&sprite, screen, x, y, 1);
    refreshPartialScreen(screen, 3, 4, x, y);

    while(1){
        args->SystemTable->ConIn->ReadKeyStroke(args->SystemTable->ConIn, &k);
        if(k.ScanCode == 0x02){if(y+1 <= h){refreshSpritePosition(&sprite, screen, x, y, x, y+1, 255, 1); y++;}}
        if(k.ScanCode == 0x01){if(y-1 >= 0){refreshSpritePosition(&sprite, screen, x, y, x, y-1, 255, 1); y--;}}
        if(k.ScanCode == 0x03){if(x+1 <= w){refreshSpritePosition(&sprite, screen, x, y, x+1, y, 255, 1); x++;}}
        if(k.ScanCode == 0x04){if(x-1 >= 0){refreshSpritePosition(&sprite, screen, x, y, x-1, y, 255, 1); x--;}}
        drawSpriteOnScreen(&sprite, screen, x , y, 1);
        drawScreen(screen);

    }
    FreePool(screen->screen);
    FreePool(screen);
	return 0;
}

#endif