#ifndef GRAPHICS_H_
#define GRAPHICS_H_
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>
#include "std.h"
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
void drawScreen(SCREEN *screen);
void drawSpriteOnScreen(SPRITE *sprite, SCREEN *screen, UINT16 x, UINT16 y, UINT8 scale);
void refreshPartialScreen(SCREEN *screen, UINT16 w, UINT16 h, UINT16 x, UINT16 y);
void refreshSpritePosition(SPRITE *sprite, SCREEN *screen, UINT16 oldx, UINT16 oldy, UINT16 newx, UINT16 newy, UINT8 colorfill, UINT8 scale);
void clearScreen(SCREEN *screen, UINT8 colorfill);

#endif
