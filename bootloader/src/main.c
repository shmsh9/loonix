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
	struct fnargs args = {0};
	args.SystemTable = SystemTable;
	args.ImageHandle = ImageHandle;
	args.printfn = (void *)Print;
	//Configure Serial port
	/*
	efi_status_t status = 0;
	EFI_SERIAL_IO_PROTOCOL *serial_protocol = NULL;
	struct efi_guid serial_guid = EFI_SERIAL_IO_PROTOCOL_GUID;
	status = SystemTable->boot->open_protocol(ImageHandle, &serial_guid, (void **)&serial_protocol, ImageHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
	if(status == 0){
		Print(L"protocol opened\n");
		serial_protocol->SetAttributes(serial_protocol, 0, 0, 0, 0, 0, DefaultStopBits);
		char buff[] = "hello from serial\n";
		efi_uint_t szbuff = sizeof(buff);
		serial_protocol->Write(serial_protocol, &szbuff, (void *)buff);
	}
	else{
		Print(L"serial_protocol error : status == %d\n", status);
	}
	*/
	Print(L"loading kernel\n");
	//loadelf(L"kernel.elf", &args);
	uint64_t ret = __loadelf_with_no_return(L"kernel.elf", &args);
	Print(L"rip kernel x_x : 0x%x\n", ret);
	while (1)
	{
		/* code */
	}
	
	return EFI_SUCCESS;
}
