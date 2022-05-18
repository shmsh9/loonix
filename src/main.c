/*
 * UEFI:SIMPLE - UEFI development made easy
 * Copyright ©️ 2014-2021 Pete Batard <pete@akeo.ie> - Public Domain
 * See COPYING for the full licensing terms.
 */
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>
#include <shell.h>
#include <stack.h>
#include <std.h>
/*
#if defined(_M_X64) || defined(__x86_64__)

static CHAR16* Arch = L"x64";
static CHAR16* ArchName = L"64-bit x86";

#elif defined(_M_IX86) || defined(__i386__)
static CHAR16* Arch = L"ia32";
static CHAR16* ArchName = L"32-bit x86";
#elif defined (_M_ARM64) || defined(__aarch64__)
static CHAR16* Arch = L"aa64";
static CHAR16* ArchName = L"64-bit ARM";
#elif defined (_M_ARM) || defined(__arm__)
static CHAR16* Arch = L"arm";
static CHAR16* ArchName = L"32-bit ARM";
#else
#  error Unsupported architecture
#endif
*/
//END BUILTINS
// Application entrypoint (must be set to 'efi_main' for gnu-efi crt0 compatibility)
struct stack *usralloc;
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
#if defined(_GNU_EFI)
	InitializeLib(ImageHandle, SystemTable);
#endif
	usralloc = kmalloc(sizeof(struct stack));
	shell(ImageHandle, SystemTable);
	return EFI_SUCCESS;
}
