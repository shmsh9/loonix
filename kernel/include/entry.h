#ifndef ENTRY_H_
#define ENTRY_H_
#include <efi.h>
#include <shell.h>
#include <efibind.h>
#include <eficompiler.h>
EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
EFI_RUNTIME_SERVICES *RuntimeServices;
extern int main(int argc, char **argv);

int entry(struct fnargs *fnargs){
	fnargs->SystemTable->ConOut->OutputString(fnargs->SystemTable->ConOut, L"henlo\n");
//	return main(fnargs->argc, fnargs->charargv); 
	return 0xdeadcaca;
}
#endif
