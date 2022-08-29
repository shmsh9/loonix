/*
 * UEFI:SIMPLE - UEFI development made easy
 * Copyright ©️ 2014-2021 Pete Batard <pete@akeo.ie> - Public Domain
 * See COPYING for the full licensing terms.
 */
#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>
#include <elf.h>
#include <std.h>
#include <bootloader.h>
/*
 Put all the ugly globals here
 */
EFI_HANDLE      ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;

VOID bootloader_timeout(IN EFI_EVENT  Event, IN VOID *Context){
	loadelf(L"kernel.elf", (bootinfo *)Context);
}

EFI_STATUS efi_main(EFI_HANDLE aImageHandle, EFI_SYSTEM_TABLE *aSystemTable){
	ImageHandle = aImageHandle;
	SystemTable = aSystemTable;
	SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	bootinfo bootinfo = {
		.ImageHandle = ImageHandle, 
		.SystemTable = SystemTable, 
		.RuntimeServices = SystemTable->RuntimeServices, 
		.kernelbase = 0, 
		.kernelentry = 0, 
		.kernelsize = 0, 
		.mmap = 0, 
		.mmap_size = 0,
		.mmap_key = 0,
		.uefi_exit_code = 0,
		.acpi_table = SystemTable->ConfigurationTable,
		.acpi_table_length = SystemTable->NumberOfTableEntries,
		.framebuffer = {0}
	};
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	EFI_STATUS s = SystemTable->BootServices->LocateProtocol(&gopGuid, NULL, (void **)&gop);
	if(EFI_ERROR(s)){
		DEBUG(L"can not open gop protocol : 0x%x !", s);
	}
	else{
    	bootinfo.framebuffer.address =  gop->Mode->FrameBufferBase;
    	bootinfo.framebuffer.size = gop->Mode->FrameBufferSize;
    	bootinfo.framebuffer.height = gop->Mode->Info->VerticalResolution;
		//fix huawei buggy mode resolutions
		bootinfo.framebuffer.width = gop->Mode->Info->PixelsPerScanLine;
    	//bootinfo.framebuffer.width = gop->Mode->Info->HorizontalResolution;
		EFI_EVENT ev = 0;
		UINTN to = 10;
		SystemTable->BootServices->CreateEvent(
			EVT_TIMER | EVT_NOTIFY_SIGNAL,
			TPL_CALLBACK,
			bootloader_timeout,
			&bootinfo,
			&ev
		);
		SystemTable->BootServices->SetTimer(
			ev,
			TimerRelative,
			to*10000000
		);
		CHAR16 *padding = L" ";
/*
		CHAR16 *banner = L""         
L"         _nnnn_ \n"
L"        dGGGGMMb \n"
L"       @p~qp~~qMb \n"   
L"       M|@||@) M| \n"   
L"       @,----.JM| \n"   
L"      JS^\\__/  qKL \n"
L"     dZP        qKRb \n" 
L"    dZP          qKKb \n" 
L"   fZP            SMMb \n"
L"   HZM            MMMM \n"
L"   FqM            MMMM \n"
L" __| \".        |\\dS\"qML \n"
L" |    `.       | `' \\Zq \n"
L"_)      \\.___.,|     .' \n"
L"\\____   )MMMMMP|   .' \n"
L"     `-'       `--' \n";
		Print(L"%s\n", banner);
*/
		Print(L"[L00n1x bootloader]\n\n");
		for(uint8_t i = 0; i < gop->Mode->MaxMode; i++){
  			EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info = 0;
  			UINTN SizeOfInfo = 0;
			EFI_STATUS t = gop->QueryMode(gop, i, &SizeOfInfo, &info);
			CHAR16 *space_mode = i < 10 ? L"  ": L" ";
			UINTN res_len = 0;
			if(info->HorizontalResolution < 1000){
				res_len += 3;
			}
			else{
				res_len += 4;
			}
			if(info->VerticalResolution < 1000){
				res_len += 3;
			}
			else{
				res_len += 4;
			}
			CHAR16 *space_res = res_len == 8 ? L" ": L"  ";
			if(res_len == 6)
				space_res = L"   ";
			if(t != EFI_SUCCESS)
				continue;
			Print(L"%s %d %s: %dx%d %s",
				padding,
				i,
				space_mode,
				info->HorizontalResolution, 
				info->VerticalResolution,
				space_res
			);
			if(!((i+1) % 4))
				Print(L"\n");
		}
		Print(L"\n\n");
		EFI_INPUT_KEY k = {0};
		UINTN KeyEvent = 0;
		uint8_t mode = 0;
		Print(L"\rSelect GOP mode (%d sec timeout) : %d",to, mode);
		while(k.UnicodeChar != 0xd){
			SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &KeyEvent);
			SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &k);
			if(k.UnicodeChar - L'0' >= 0 && k.UnicodeChar - L'0' < 10){
				mode *= 10;
				mode += k.UnicodeChar - L'0';
			}
			Print(L"\rSelect GOP mode (%d sec timeout) : %d",to, mode);
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
