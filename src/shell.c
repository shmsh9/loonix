/*
 * UEFI:SIMPLE - UEFI development made easy
 * Copyright ©️ 2014-2021 Pete Batard <pete@akeo.ie> - Public Domain
 * See COPYING for the full licensing terms.
 */
#include "shell.h"
void test(){
	Print(L"TestFN\n");
}
int shell_exec(struct fnargs *args){
struct fnstruct fn[] = {
		{L"fart", 			L"Farting on you", 			fart},
		{L"clear",			L"Clear the screen", 		clear},
		{L"testargs", 	L"Testing function",		testargs},
		{L"fbinit", 		L"Init framebuffer",		fbinit},
		{L"drawpx", 		L"Draw pixel",					drawpx},
		{L"lame", 			L"Lame game",						lamegame},
		{L"ls",					L"List files",					ls},
		{L"date",				L"Get time",						date},
		{L"exit", 			L"Exit l00n1x" ,				exitshell},
		{L"elf",				L"Load elf", 						elfmain}
	
};
	args->argc = parseargs(args->stdin,args->argv);
	Print(L"\n");
	if(StrCmp(args->argv[0], L"help") == 0){
		for(int i = 0; i <sizeof(fn)/sizeof(fn[0]); i++){
			Print(L"%s :     %s\n", fn[i].name, fn[i].description);
		}
		return 0;
	}
	for(int i = 0; i < sizeof(fn)/sizeof(fn[0]); i++){
		if(StrCmp(args->argv[0], fn[i].name) == 0){
			return (int)fn[i].function(args);
		}
	}
	Print(L"shewax : %s : command not found", args->argv[0]);
	return -1;
}
//END BUILTINS
// Application entrypoint (must be set to 'efi_main' for gnu-efi crt0 compatibility)
EFI_STATUS shell(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
	#define BANNER L"------------------------\nWelcome to l00n1x\nThe best OS\n------------------------\n"
	#define PROMPT L"l00n1x $> "
	//Print(BANNER);
	UINTN Event;
	uefi_call_wrapper(SystemTable->ConOut->EnableCursor, 2,SystemTable->ConOut, TRUE);
	CHAR16 *buff = calloc(CMD_BUFF_SIZE, sizeof(CHAR16));
	CHAR16 *prevbuff = calloc(CMD_BUFF_SIZE, sizeof(CHAR16));
	struct fnargs *args = calloc(sizeof(struct fnargs), 1);
	CHAR16 tmp[2];
	args->ImageHandle = ImageHandle;
	args->SystemTable = SystemTable;
	args->stdin = buff;
	SIMPLE_INPUT_INTERFACE *stdin = SystemTable->ConIn;
	EFI_INPUT_KEY k = {0};
	Print(PROMPT);
	while(1){
		uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, stdin, &k);
		//backspace =  8
		if(k.ScanCode == 0x08){
			if(buff[0] != L'\0'){
				Print(L"%c", 0x08);
				Print(L"%c", L' ');
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
			uefi_call_wrapper(SystemTable->ConOut->SetCursorPosition,3,SystemTable->ConOut, SystemTable->ConOut->Mode->CursorColumn+1, SystemTable->ConOut->Mode->CursorRow);
		}
		//0x04 = left arrow
		if(k.ScanCode == 0x04){
			if(SystemTable->ConOut->Mode->CursorColumn > sizeof(PROMPT)/sizeof(PROMPT[0]))
				uefi_call_wrapper(SystemTable->ConOut->SetCursorPosition,3,SystemTable->ConOut, SystemTable->ConOut->Mode->CursorColumn-1, SystemTable->ConOut->Mode->CursorRow);
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
		cleanargs(args->argc, args->argv);
		args->argc = 0;
	}
	uefi_call_wrapper(SystemTable->ConIn->Reset, 2, SystemTable->ConIn, FALSE);
	uefi_call_wrapper(SystemTable->BootServices->WaitForEvent,3,1, &SystemTable->ConIn->WaitForKey, &Event);
#if defined(_DEBUG)
	// If running in debug mode, use the EFI shut down call to close QEMU
	uefi_call_wrapper(SystemTable->RuntimeServices->ResetSystem, 4, EfiResetShutdown, EFI_SUCCESS, 0, NULL);
#endif

	return EFI_SUCCESS;
}
int parseargs(CHAR16 *stdin, CHAR16 **argv){
	int ret = 1;
	int l = StrnLen(stdin, CMD_BUFF_SIZE);
	for(int i = 0; i < l; i++){
		if(stdin[i] == L' '){
			for(int j = i; j < l; j++)
				if(stdin[j] != L' '){
					ret++;
					i = j;
					break;
				}
		}
	}
	if(ret > 255){
		ret = 1;
		Print(L"shewax : error : to much args");
		return ret;
	}
	CHAR16 *tmp = calloc(l,sizeof(CHAR16));
	tmp[0] = L'\0';
	int i = 0;
	int k = 0;
	int x = 0;
	for(; i < l; i++, k++){
		if(stdin[i] == L' '){
			tmp[k] = L'\0';
			k = 0;
			argv[x] = StrDuplicate(tmp);
			x++;
			for(int j = i; j < l; j++)
				if(stdin[j] != L' '){
					i = j;
					break;
				}
		}
		tmp[k] = stdin[i];
	}
	if(ret > 1){
		tmp[k] = L'\0';
		argv[x] = StrDuplicate(tmp);
	}
	else{
		argv[0] = StrDuplicate(stdin);
	}
	free(tmp);
	return ret;
}
void cleanargs(int argc, CHAR16 **argv){
	for(int i = 0; i < argc; i++){
		FreePool(argv[i]);
	}
	//FreePool(argv);
}
int exitshell(struct fnargs *args){
	Exit(EFI_SUCCESS, 0, NULL);
	return 0;
}

int testargs(struct fnargs *args){
	Print(L"argc == %d\n", args->argc);
	for(int i = 0; i < args->argc; i++){
		Print(L"argv[%d] == %s\n", i, args->argv[i]);
	}
	return 0;
}
int fart(struct fnargs *args){
	Print(L"Nuage de fart de nene (-.-)");
	return 0;
}
int clear(struct fnargs *args){
	uefi_call_wrapper(args->SystemTable->ConOut->ClearScreen, 1, args->SystemTable->ConOut);
	return 0;
}
int date(struct fnargs *args){
	EFI_TIME Time;
	uefi_call_wrapper(gRT->GetTime,2,&Time, NULL);
	Print(L"%d/%d/%d %d:%d:%d UTC", Time.Day, Time.Month, Time.Year, Time.Hour, Time.Minute, Time.Second);
	return 0;
}
int fbinit(struct fnargs *args){
	uefi_call_wrapper(args->SystemTable->ConOut->Reset,2,args->SystemTable->ConOut, FALSE);
	uefi_call_wrapper(args->SystemTable->ConOut->SetAttribute,2,args->SystemTable->ConOut, EFI_TEXT_ATTR(EFI_BLUE, EFI_YELLOW));
	uefi_call_wrapper(args->SystemTable->ConOut->ClearScreen,1,args->SystemTable->ConOut);

	return EFI_SUCCESS;
	
}
int drawpx(struct fnargs *args){
//	fbinit(args);
	EFI_STATUS rc;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    rc = LibLocateProtocol(&GraphicsOutputProtocol, (void **)&gop);	
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixels[256];
	for(int i = 0; i < 256; i++){
		pixels[i].Blue = 255-i;
		pixels[i].Red = 240-i;
		pixels[i].Green = 230-i;
		pixels[i].Reserved = 0;
		rc = uefi_call_wrapper(gop->Blt, 10, gop, (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)&pixels, EfiBltVideoFill, 0, 0, 0, 0, 10, 10, 0);

	}
	return rc;
}
int ls(struct fnargs *args){
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
  EFI_FILE_PROTOCOL *RootDir;
  UINT8 Buffer[1024];
  UINTN BufferSize;
  EFI_FILE_INFO *FileInfo;
  uefi_call_wrapper(BS->LocateProtocol, 3, &FileSystemProtocol, NULL, (void **)&FileSystem);
  uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &RootDir);
  while (TRUE) {
  	BufferSize = sizeof(Buffer);
    uefi_call_wrapper(RootDir->Read, 3, RootDir, &BufferSize, Buffer);
    if (BufferSize == 0) {
    	break;
    }
		FileInfo = (EFI_FILE_INFO *)Buffer;
    Print(L"%s\n", FileInfo->FileName);
	}
  uefi_call_wrapper(RootDir->Close, 1, RootDir);
	return 0;
}

