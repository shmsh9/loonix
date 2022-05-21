#ifndef STDLIB_H_
#define STDLIB_H_
#include <efibind.h>
#include <eficompiler.h>
#include <shell.h>
#include <stdlib_h.h>
#include <string.h>
efi_handle_t ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
extern int main(int argc, CHAR16 **argv);

int entry(struct fnargs *fnargs){
	ImageHandle = fnargs->ImageHandle;
	SystemTable = fnargs->SystemTable;
	FileSystem = fnargs->FileSystem;
	return main(fnargs->argc, fnargs->argv); 
}

#endif
