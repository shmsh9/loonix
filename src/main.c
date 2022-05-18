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
struct stack *usralloc;
struct syscall *syscalls;
struct syscall syscalltable[] = {
	{read},
	{write},
	{open},
	{sysmalloc},
	{sysfree}
};

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
#if defined(_GNU_EFI)
	InitializeLib(ImageHandle, SystemTable);
#endif
	usralloc = kcalloc(1,sizeof(struct stack));
	syscalls = syscalltable;
	shell(ImageHandle, SystemTable);
	kfree(usralloc);
	return EFI_SUCCESS;
}
