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
void __internalprint(CHAR16 *str);
FILE *fopen(CHAR16 *filename, const CHAR16 *mode);
size_t fread(void *buffer, size_t size, size_t count, FILE *f);	
size_t fclose(FILE *f);
size_t fsize(FILE *f);
EFI_INPUT_KEY getchar();
#endif
