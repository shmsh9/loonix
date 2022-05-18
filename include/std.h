#ifndef STD_H_
#define STD_H_
#include <efi.h>
#include <efilib.h>
#define kfree(ptr) FreePool(ptr)
typedef uintptr_t size_t;
typedef BOOLEAN bool;
typedef EFI_FILE_HANDLE FILE;

void *kmalloc(size_t sz);
void *kcalloc(size_t elementCount, size_t elementSize);
FILE kfopen(CHAR16 *path, CHAR16 *mode, EFI_HANDLE *image);
void kfclose(FILE f);
size_t kfsize(FILE f);
size_t kfread(void *buff, size_t szelement, size_t nbelement, FILE f);
#endif
