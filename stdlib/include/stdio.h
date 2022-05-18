#ifndef STDIO_H_
#define STDIO_H_
#include <type.h>
#include <string.h>
#include <stdlib_h.h>
#include <syscall.h>
#include <efi.h>
#include <efilib.h>
typedef EFI_FILE_HANDLE FILE;
extern EFI_SYSTEM_TABLE *SystemTable;
extern EFI_HANDLE ImageHandle; 
extern struct syscall *syscalls;
void printf(CHAR16 *fmt, ...);
void putchar(CHAR16 c);
void puts(CHAR16 *s);
void print(CHAR16 *s);
EFI_INPUT_KEY getchar();
#endif
