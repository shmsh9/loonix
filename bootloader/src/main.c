/*
 * UEFI:SIMPLE - UEFI development made easy
 * Copyright ©️ 2014-2021 Pete Batard <pete@akeo.ie> - Public Domain
 * See COPYING for the full licensing terms.
 */
#include <efi.h>
#include <shell.h>
#include <stack.h>
#include <elf.h>
#include <std.h>
/*
 Put all the ugly globals here
 */
struct stack     *usralloc;
efi_handle_t      ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;

efi_status_t efi_main(efi_handle_t aImageHandle, EFI_SYSTEM_TABLE *aSystemTable){
	ImageHandle = aImageHandle;
	SystemTable = aSystemTable;
#if defined(_GNU_EFI)
	InitializeLib(ImageHandle, SystemTable);
#endif
	//removes 5min timeout
	usralloc = kcalloc(1,sizeof(struct stack));
	/*
	uefi_call_wrapper(SystemTable->BootServices->SetWatchdogTimer,4,0, 0, 0, NULL);
	uefi_call_wrapper(SystemTable->ConOut->SetAttribute, 2, SystemTable->ConOut, EFI_BACKGROUND_BLACK);
	uefi_call_wrapper(SystemTable->ConOut->SetAttribute, 2, SystemTable->ConOut, EFI_WHITE);
	uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut);
	shell();
	*/
	struct fnargs args;
	args.argv[0] = L"elf";
	args.argc = 1;
	args.SystemTable = SystemTable;
	args.ImageHandle = ImageHandle;
	SystemTable->out->output_string(SystemTable->out, L"Loading kernel\n");
	elfshell(L"kernel.elf", &args);
	while (1)
	{
	}
	
	kfree(usralloc);
	return EFI_SUCCESS;
}
