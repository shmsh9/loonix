/*
 * UEFI:SIMPLE - UEFI development made easy
 * Copyright ©️ 2014-2021 Pete Batard <pete@akeo.ie> - Public Domain
 * See COPYING for the full licensing terms.
 */
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>
#include <syscall.h>
#include <shell.h>
#include <stack.h>
#include <std.h>
/*
 Put all the ugly globals here
 */
struct stack     *usralloc;
struct syscall   *syscalls;
EFI_HANDLE       ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;

struct syscall syscalltable[] = {
	{read},
	{write},
	{open},
	{close},
	{sysfsize},
	{sysmalloc},
	{sysfree},
	{syselfload},
	{sysprint},
	{sysreadkey}
	
};


EFI_STATUS efi_main(EFI_HANDLE aImageHandle, EFI_SYSTEM_TABLE *aSystemTable){
	ImageHandle = aImageHandle;
	SystemTable = aSystemTable;
#if defined(_GNU_EFI)
	InitializeLib(ImageHandle, SystemTable);
#endif
	uefi_call_wrapper(BS->LocateProtocol, 3, &FileSystemProtocol, NULL, (void **)&FileSystem);
	usralloc = kcalloc(1,sizeof(struct stack));
	syscalls = syscalltable;
	shell();
	kfree(usralloc);
	return EFI_SUCCESS;
}
