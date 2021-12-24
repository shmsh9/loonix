#ifndef BUILTIN_H_
#define BUILTIN_H_

#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>
#include "helpers.h"
#include "magic.h"
int exit(struct fnargs *args);
int testargs(struct fnargs *args);
int fart(struct fnargs *args);
int clear(struct fnargs *args);
int date(struct fnargs *args);
int fbinit(struct fnargs *args);
int drawpx(struct fnargs *args);

int exit(struct fnargs *args){
	Exit(EFI_SUCCESS, 0, NULL);
	return 0;
}
int testargs(struct fnargs *args){
	CHAR16 **argv = NULL;
	int argc = parseargs(args->stdin, argv);
	Print(L"argc == %d\n", argc);
	for(int i = 0; i < argc; i++){
		Print(L"argv[%d] == %s\n", i, argv[i]);
	}
	cleanargs(argc, argv);
	return 0;
}
int fart(struct fnargs *args){
	Print(L"Nuage de fart de nene (-.-)");
	return 0;
}
int clear(struct fnargs *args){
	args->SystemTable->ConOut->ClearScreen(args->SystemTable->ConOut);
	return 0;
}
int date(struct fnargs *args){
	EFI_TIME Time;
	gRT->GetTime(&Time, NULL);
	Print(L"%d/%d/%d %d:%d:%d UTC", Time.Day, Time.Month, Time.Year, Time.Hour, Time.Minute, Time.Second);
	return 0;
}
int fbinit(struct fnargs *args){
	args->SystemTable->ConOut->Reset(args->SystemTable->ConOut, FALSE);
	args->SystemTable->ConOut->SetAttribute(args->SystemTable->ConOut, EFI_TEXT_ATTR(EFI_BLUE, EFI_YELLOW));
	args->SystemTable->ConOut->ClearScreen(args->SystemTable->ConOut);

	return EFI_SUCCESS;
	
}
int drawpx(struct fnargs *args){
//	fbinit(args);
	EFI_STATUS rc;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    rc = LibLocateProtocol(&GraphicsOutputProtocol, (void **)&gop);	
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixels[256];
	for(int i = 0; i < 256; i++){
		pixels[i].Blue = 255-i;
		pixels[i].Red = 240-i;
		pixels[i].Green = 230-i;
		pixels[i].Reserved = 0;
		rc = uefi_call_wrapper(gop->Blt, 10, gop, (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)&pixels, EfiBltVideoFill, 0, 0, 0, 0, 10, 10, 0);

	}
	return rc;
}

#endif