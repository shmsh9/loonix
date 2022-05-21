#ifndef ENTRY_H_
#define ENTRY_H_
#include <efi.h>
#include <shell.h>

EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
EFI_RUNTIME_SERVICES *RuntimeServices;
extern int main(int argc, char **argv);

int EFIAPI entry(struct fnargs *fnargs){
	ImageHandle = fnargs->ImageHandle;
	SystemTable = fnargs->SystemTable;
	FileSystem = fnargs->FileSystem;
	RuntimeServices = fnargs->RuntimeServices;
	return main(fnargs->argc, fnargs->charargv); 
}
#endif