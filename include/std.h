#ifndef STD_H_
#define STD_H_
#include <efi.h>
#include <efilib.h>
#define free(ptr) FreePool(ptr)
#ifdef	__amd64__
typedef unsigned long long size_t;
#endif
#ifdef __aarch64__
typedef unsigned long size_t;
#endif
typedef BOOLEAN bool;
typedef EFI_FILE_HANDLE FILE;

void *malloc(size_t sz);
void *calloc(size_t elementCount, size_t elementSize);
FILE fopen(CHAR16 *path, CHAR16 *mode, EFI_HANDLE *image);
void fclose(FILE f);
size_t fsize(FILE f);
size_t fread(void *buff, size_t szelement, size_t nbelement, FILE f);
#endif
