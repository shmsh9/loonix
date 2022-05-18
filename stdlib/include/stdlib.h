#ifndef STDLIB_H_
#define STDLIB_H_
#include <syscall.h>
#include <shell.h>
#include <stdlib_h.h>
struct syscall *syscalls;
EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;

extern int main(int argc, CHAR16 **argv);
int __attribute__((ms_abi)) entry(struct fnargs *fnargs){
	syscalls = fnargs->syscalls;
	ImageHandle = fnargs->ImageHandle;
	SystemTable = fnargs->SystemTable;
	FileSystem = fnargs->FileSystem;
	return main(fnargs->argc, fnargs->argv); 
}

#endif
