/*
 * UEFI:SIMPLE - UEFI development made easy
 * Copyright ©️ 2014-2021 Pete Batard <pete@akeo.ie> - Public Domain
 * See COPYING for the full licensing terms.
 */
#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>
#include <elf.h>
#include <std.h>
/*
 Put all the ugly globals here
 */
EFI_HANDLE      ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;

EFI_STATUS efi_main(EFI_HANDLE aImageHandle, EFI_SYSTEM_TABLE *aSystemTable){
	ImageHandle = aImageHandle;
	SystemTable = aSystemTable;
	SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
	bootinfo bootinfo = {
		ImageHandle, 
		SystemTable, 
		SystemTable->RuntimeServices, 
		0, 
		0, 
		0, 
		0, 
		0,
		0,
		0,
		{0}
	};
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	EFI_STATUS s = SystemTable->BootServices->LocateProtocol(&gopGuid, NULL, (void **)&gop);
	if(s){
		DEBUG(L"can not open gop protocol : 0x%x !", s);
	}
	else{
		for(uint8_t i = 0; i < gop->Mode->MaxMode; i++){
  			EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info = 0;
  			UINTN SizeOfInfo = 0;
			EFI_STATUS t = gop->QueryMode(gop, i, &SizeOfInfo, &info);
			if(t != EFI_SUCCESS)
				continue;
			Print(L"mode %d : %dx%d pixel_format : %d pixel_per_line : %d\n", 
				i, 
				info->HorizontalResolution, 
				info->VerticalResolution,
				info->PixelFormat,
				info->PixelsPerScanLine
			);
		}
		EFI_INPUT_KEY k = {0};
		UINTN KeyEvent = 0;
		uint8_t mode = 0;
		Print(L"Select GOP mode : ");
		while(k.UnicodeChar != 0xd){
			SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &KeyEvent);
			SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &k);
			if(k.UnicodeChar - L'0' >= 0 && k.UnicodeChar - L'0' < 10){
				mode *= 10;
				mode += k.UnicodeChar - L'0';
				CHAR16 tmp[2] = {k.UnicodeChar, 0};
				Print(tmp);
			}
		}
		Print(L"\n");
		if(mode < gop->Mode->MaxMode)
			gop->SetMode(gop, mode);
    	bootinfo.framebuffer.address =  gop->Mode->FrameBufferBase;
    	bootinfo.framebuffer.size = gop->Mode->FrameBufferSize;
    	bootinfo.framebuffer.height = gop->Mode->Info->VerticalResolution;
		//fix huawei buggy mode resolutions
		bootinfo.framebuffer.width = gop->Mode->Info->PixelsPerScanLine;
    	//bootinfo.framebuffer.width = gop->Mode->Info->HorizontalResolution;
	}
	uint64_t ret = loadelf(L"kernel.elf", &bootinfo);
	DEBUG(L"rip kernel x_x : 0x%x\n", ret);
	while (1){}
	
	return EFI_SUCCESS;
}
