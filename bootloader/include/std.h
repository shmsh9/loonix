#ifndef STD_H_
#define STD_H_
#include <efi.h>
#include <efilib.h>
#define kfree(ptr) FreePool(ptr)
typedef uintptr_t size_t;
typedef BOOLEAN bool;
typedef EFI_FILE_HANDLE FILE;

void   EFIAPI *kmalloc(size_t sz);
void   EFIAPI *kcalloc(size_t elementCount, size_t elementSize);
FILE   EFIAPI kfopen(CHAR16 *path, CHAR16 *mode, EFI_HANDLE *image);
void   EFIAPI kfclose(FILE f);
size_t EFIAPI kfsize(FILE f);
size_t EFIAPI kfread(void *buff, size_t szelement, size_t nbelement, FILE f);
#endif
