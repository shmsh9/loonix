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
		//gop->SetMode(gop, 10);
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
