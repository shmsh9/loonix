#ifndef APP_H_
#define APP_H_
#include "syscall.h"
#include "shell.h"
struct syscall *syscalls;
EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
extern int main(int argc, CHAR16 **argv);

void printf(CHAR16 *fmt, ...){
	va_list args;
	va_start(args, fmt);
	uefi_call_wrapper(SystemTable->ConOut->OutputString,2,SystemTable->ConOut, fmt);
	va_end(args);
}
size_t entry(struct fnargs *fnargs){
	syscalls = fnargs->syscalls;
	ImageHandle = fnargs->ImageHandle;
	SystemTable = fnargs->SystemTable;
	FileSystem = fnargs->FileSystem;
	return main(fnargs->argc, fnargs->argv); 
}

void memset(void *dst, char val, size_t sz){
	for(int i = 0; i < sz; i++)
		((uint8_t *)dst)[i] = val;
}
void memcpy(void *dst, void *src, size_t sz){
	for(int i = 0; i < sz; i++)
		((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
}

#endif
