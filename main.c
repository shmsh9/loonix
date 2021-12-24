/*
 * UEFI:SIMPLE - UEFI development made easy
 * Copyright ©️ 2014-2021 Pete Batard <pete@akeo.ie> - Public Domain
 * See COPYING for the full licensing terms.
 */
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>
#include "magic.h"
#include "builtin.h"
#include "lamegame.h"

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



int shell_exec(struct fnargs *args){
	struct fnstruct fn[] = {
		{L"fart", 		L"Farting on you", 		fart},
		{L"clear",		L"Clear the screen", 	clear},
		{L"testargs", 	L"Testing function", 	testargs},
		{L"fbinit", 	L"Init framebuffer",	fbinit},
		{L"drawpx", 	L"Draw pixel",			drawpx},
		{L"lame", 		L"Lame game",			lamegame},

		{L"date",		L"Get time",			date},
		{L"exit", 		L"Exit l00n1x" ,		exit}
	};
	args->argv = NULL;
	args->argc = 0;
	CHAR16 *argv0;
	gBS->AllocatePool(EfiBootServicesData, CMD_BUFF_SIZE*sizeof(CHAR16), (void *)&argv0);
	SetMem(argv0, CMD_BUFF_SIZE*sizeof(CHAR16), 0);

	for(int i = 0; i < StrnLen(args->stdin, CMD_BUFF_SIZE); i++){
		if(args->stdin[i] == L' ')
			break;
		argv0[i] =  args->stdin[i];	
	}
	Print(L"\n");
	if(StrCmp(argv0, L"help") == 0){
		FreePool(argv0);
		for(int i = 0; i <sizeof(fn)/sizeof(fn[0]); i++){
			Print(L"%s :     %s\n", fn[i].name, fn[i].description);
		}
		return 0;
	}
	for(int i = 0; i < sizeof(fn)/sizeof(fn[0]); i++){
		if(StrCmp(argv0, fn[i].name) == 0){
			FreePool(argv0);
			return fn[i].function((struct fnargs *)args);
		}
	}
	Print(L"shewax : %s : command not found", argv0);
	FreePool(argv0);
	return -1;
}
//END BUILTINS
// Application entrypoint (must be set to 'efi_main' for gnu-efi crt0 compatibility)
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	UINTN Event;

#if defined(_GNU_EFI)
	InitializeLib(ImageHandle, SystemTable);
#endif

	// The platform logo may still be displayed → remove it
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	/*
	 * In addition to the standard %-based flags, Print() supports the following:
	 *   %N       Set output attribute to normal
	 *   %H       Set output attribute to highlight
	 *   %E       Set output attribute to error
	 *   %r       Human readable version of a status code
	 */
	#define BANNER L"------------------------\nWelcome to l00n1x\nThe best OS\n------------------------\n"
	#define PROMPT L"l00n1x $> "
	Print(BANNER);
	SystemTable->ConOut->EnableCursor(SystemTable->ConOut, TRUE);
	CHAR16 *buff;
	CHAR16 *prevbuff = NULL;
	struct fnargs *args = NULL;
	//stores the char to append to the command buffer
	CHAR16 tmp[2];
	gBS->AllocatePool(EfiBootServicesData, CMD_BUFF_SIZE*sizeof(CHAR16), (void *)&buff);
	gBS->AllocatePool(EfiBootServicesData, CMD_BUFF_SIZE*sizeof(CHAR16), (void *)&prevbuff);
	gBS->AllocatePool(EfiBootServicesData, sizeof(struct fnargs), (void *)&args);
	args->ImageHandle = ImageHandle;
	args->SystemTable = SystemTable;
	args->stdin = buff;
	SetMem(buff, CMD_BUFF_SIZE, 0);

	SIMPLE_INPUT_INTERFACE *stdin = SystemTable->ConIn;
	EFI_INPUT_KEY k = {0};
	Print(PROMPT);
	while(1){
		SystemTable->ConIn->ReadKeyStroke(stdin, &k);
		//backspace =  8
		if(k.UnicodeChar == 0x08){
			if(buff[0] != L'\0'){
				Print(L"%c", 0x08);
				buff[StrnLen(buff, CMD_BUFF_SIZE)-1] = L'\0';
			}
		}
		//0x01 = up arrow
		if(k.ScanCode == 0x01){
			for(int j = 0; j < StrnLen(buff, CMD_BUFF_SIZE); j++){
				Print(L"%c", 0x08);
			}
			SetMem(buff, CMD_BUFF_SIZE*sizeof(CHAR16), 0);
			StrCpy(buff, prevbuff);
			Print(buff);
		}
		//0x04 = right arrow
		if(k.ScanCode == 0x03){
			SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, SystemTable->ConOut->Mode->CursorColumn+1, SystemTable->ConOut->Mode->CursorRow);
		}
		//0x04 = left arrow
		if(k.ScanCode == 0x04){
			if(SystemTable->ConOut->Mode->CursorColumn > sizeof(PROMPT)/sizeof(PROMPT[0]))
				SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, SystemTable->ConOut->Mode->CursorColumn-1, SystemTable->ConOut->Mode->CursorRow);
		}
		//return = UnicodeChar 13
		if(k.UnicodeChar == 13){
			SetMem(prevbuff,CMD_BUFF_SIZE*sizeof(CHAR16), 0);
			StrCpy(prevbuff, buff);
			shell_exec(args);
			SetMem(buff, CMD_BUFF_SIZE*sizeof(CHAR16), 0);
			Print(L"\n%s", PROMPT);
		}
		if(k.UnicodeChar >= 32  && k.UnicodeChar <= 126){
			SPrint(tmp, 1, L"%c", k.UnicodeChar);
			if(buff[0] == L'\0')
				StrnCpy(buff, tmp, CMD_BUFF_SIZE);
			else
				StrnCat(buff, tmp, CMD_BUFF_SIZE);
			Print(tmp);
		}
	}
	SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
	SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
#if defined(_DEBUG)
	// If running in debug mode, use the EFI shut down call to close QEMU
	SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
#endif

	return EFI_SUCCESS;
}

