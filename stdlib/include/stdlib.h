#ifndef STDLIB_H_
#define STDLIB_H_
#include <efibind.h>
#include <eficompiler.h>
#include <syscall.h>
#include <shell.h>
#include <stdlib_h.h>
#include <string.h>
struct syscall *syscalls;
struct args *syscallargs;
EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
extern int main(int argc, CHAR16 **argv);

int EFIAPI entry(struct fnargs *fnargs){
	syscalls = fnargs->syscalls;
	ImageHandle = fnargs->ImageHandle;
	SystemTable = fnargs->SystemTable;
	FileSystem = fnargs->FileSystem;
	struct args stacksyscallargs= {0,0,0,0,0,0};
	syscallargs = &stacksyscallargs;
	return main(fnargs->argc, fnargs->argv); 
}

#endif
