#ifndef STD_H_
#define STD_H_
#include <Uefi.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>
#include <extern.h>
typedef uintptr_t size_t;
typedef EFI_FILE_PROTOCOL FILE;
#define EFI_TIMER_PERIOD_SECONDS(Seconds) MultU64x32((UINT64)(Seconds), 10000000)
#define DEBUG(...) {\
	Print(L"[bootloader][debug] : ");\
	for(int DEBUGfunciterator = 0; __func__[DEBUGfunciterator]; DEBUGfunciterator++){\
		CHAR16 tmpDEBUG16[2] = {0};\
		tmpDEBUG16[0] = (CHAR16)__func__[DEBUGfunciterator];\
		Print(tmpDEBUG16);\
	}\
	Print(L"() : ");\
	Print(__VA_ARGS__);\
	Print(L"\r\n");\
}

void *kmalloc(size_t sz);
void *kcalloc(size_t elementCount, size_t elementSize);
FILE *kfopen(CHAR16 *path, CHAR16 *mode, EFI_HANDLE *image);
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
