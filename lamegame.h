#ifndef LAMEGAME_H_
#define LAMEGAME_H_
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>

#include "builtin.h"
#include "helpers.h"

EFI_GRAPHICS_OUTPUT_BLT_PIXEL icon[] = {
    {3,57,127,0},     {0,40,255,0},     {255,255,255,0},  
    {3,57,127,0},     {255,255,255,0},  {255,255,255,0},  
    {3,57,127,0},     {255,255,255,0},  {255,255,255,0},
    {3,57,127,0},     {255,255,255,0},  {0,40,255,0},  
    {3,57,127,0},     {255,255,255,0},  {0,40,255,0},  
    {3,57,127,0},     {255,255,255,0},  {0,40,255,0}


};
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
        uefi_call_wrapper(gop->Blt, 10, gop, (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)&px[i], EfiBltVideoFill, 0, 0, k*scale, j*scale, 10, 10, 0);
    }
}
int lamegame(struct fnargs *args){
	EFI_STATUS rc;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    rc = LibLocateProtocol(&GraphicsOutputProtocol, (void **)&gop);
    /*
	for(int i = 0; i < 256; i++){
		pixels[i].Blue = 255-i;
		pixels[i].Red = 240-i;
		pixels[i].Green = 230-i;
		pixels[i].Reserved = 0;
		rc = uefi_call_wrapper(gop->Blt, 10, gop, (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)&pixels, EfiBltVideoFill, 0, 0, 0, 0, 10, 10, 0);

	}
    */
    drawPX(icon, gop, 0, 0, 3, 6, 10);
	return 0;
}

#endif