/*
 * UEFI:SIMPLE - UEFI development made easy
 * Copyright ©️ 2014-2021 Pete Batard <pete@akeo.ie> - Public Domain
 * See COPYING for the full licensing terms.
 */
#include <efi.h>
#include <shell.h>
#include <elf.h>
#include <std.h>
/*
 Put all the ugly globals here
 */
efi_handle_t      ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;

efi_status_t efi_main(efi_handle_t aImageHandle, EFI_SYSTEM_TABLE *aSystemTable){
	ImageHandle = aImageHandle;
	SystemTable = aSystemTable;
	SystemTable->boot->set_watch_dog_timer(0, 0, 0, NULL);
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
	struct efi_guid gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	efi_graphics_output_protocol *gop;
	efi_status_t s = SystemTable->boot->locate_protocol(&gopGuid, NULL, (void **)&gop);
	if(s){
		DEBUG(L"can not open gop protocol : 0x%x !", s);
	}
	else{
		for(uint8_t i = 0; i < gop->Mode->MaxMode; i++){
  			EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info = 0;
  			uint32_t SizeOfInfo = 0;
			gop->QueryMode(gop, i, &SizeOfInfo, &info);
			Print(L"mode %d : %dx%d ", i, info->HorizontalResolution, info->VerticalResolution);
			if((i % 4) == 0 && i != 0)
				Print(L"\n");
		}
		Print(L"\n");
		EFI_INPUT_KEY k = {0};
		UINTN KeyEvent = 0;
		uint8_t mode = 0;
		Print(L"Select GOP mode : ");
		while(k.UnicodeChar != 0xd){
			SystemTable->boot->WaitForEvent(1, &SystemTable->in->WaitForKey, &KeyEvent);
			SystemTable->in->ReadKeyStroke(SystemTable->in, &k);
			if(k.UnicodeChar - L'0' >= 0 && k.UnicodeChar - L'0' < 10){
				mode *= 10;
				mode += k.UnicodeChar - L'0';
				CHAR16 tmp[2] = {k.UnicodeChar - L'0' , 0};
				Print(tmp);
			}
		}
		Print(L"\n");
		if(mode > gop->Mode->MaxMode)
			gop->SetMode(gop, mode);
    	bootinfo.framebuffer.address =  gop->Mode->FrameBufferBase;
    	bootinfo.framebuffer.size = gop->Mode->FrameBufferSize;
    	bootinfo.framebuffer.width = gop->Mode->Info->HorizontalResolution;
    	bootinfo.framebuffer.height = gop->Mode->Info->VerticalResolution;
	}
	uint64_t ret = loadelf(L"kernel.elf", &bootinfo);
	DEBUG(L"rip kernel x_x : 0x%x\n", ret);
	while (1){}
	
	return EFI_SUCCESS;
}
