#ifndef APP_H_
#define APP_H_
#include "syscall.h"
#include "shell.h"
struct syscall *syscalls;
EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE *SystemTable;
extern size_t main(int argc, CHAR16 **argv);
void *printfn;

void printf(IN CONST CHAR16 *fmt, ...){
	void(*ptr)(IN CONST CHAR16 *, va_list) = printfn;		
	va_list args;
	va_start(args, fmt);
	ptr(fmt, args);
	va_end(args);
}
size_t entry(struct fnargs *fnargs){
	syscalls = fnargs->syscalls;
	ImageHandle = fnargs->ImageHandle;
	SystemTable = fnargs->SystemTable;
	printfn = fnargs->printfn;
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
