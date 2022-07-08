#ifndef STD_H_
#define STD_H_
#include <efi.h>
#include <extern.h>
typedef uintptr_t size_t;
typedef struct efi_file_protocol FILE;

void *kmalloc(size_t sz);
void *kcalloc(size_t elementCount, size_t elementSize);
FILE *kfopen(CHAR16 *path, CHAR16 *mode, efi_handle_t *image);
void kfclose(FILE *f);
size_t kfsize(FILE *f);
size_t kfread(void *buff, size_t szelement, size_t nbelement, FILE *f);
void CopyMem(void *dst, void *src, uint64_t sz);
uint64_t StrCmp(CHAR16 *s1, CHAR16 *s2);
uint64_t strlen(CHAR16 *str);
void Print(CHAR16 *fmt, ...);
void SetMem(void *dst, uint8_t b, uint64_t sz);
CHAR16 *StrDuplicate(CHAR16 *str);
void * memset(void *dst, char b, size_t sz);
void * memcpy(void *dst, const void * src, size_t sz);
void kfree(void *ptr);
void *kallocaddress(size_t sz, void *address);
#endif
