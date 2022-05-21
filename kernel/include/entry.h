#ifndef ENTRY_H_
#define ENTRY_H_
#include <efi.h>
#include <shell.h>

EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
extern int main(int argc, CHAR16 **argv);

int EFIAPI entry(struct fnargs *fnargs){
	ImageHandle = fnargs->ImageHandle;
	SystemTable = fnargs->SystemTable;
	FileSystem = fnargs->FileSystem;
	return main(fnargs->argc, fnargs->argv); 
}
#endif