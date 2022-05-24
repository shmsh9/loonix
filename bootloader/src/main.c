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
	struct bootinfo bootinfo = {ImageHandle, SystemTable, 0, 0, 0, 0};
	Print(L"loading kernel\n");
	uint64_t ret = __loadelf_with_no_return(L"kernel.elf", &bootinfo);
	Print(L"rip kernel x_x : 0x%x\n", ret);
	while (1)
	{
		/* code */
	}
	
	return EFI_SUCCESS;
}
