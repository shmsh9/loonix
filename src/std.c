#include "std.h"

void *kmalloc(size_t sz){
	void *r = 0x0;
	uefi_call_wrapper(gBS->AllocatePool,3,EfiLoaderData, sz, &r);
	if(!r){
		Print(L"error : malloc() : failure\n");
	}
	return r;
}
void *kcalloc(size_t elementCount, size_t elementSize){
	void *r = kmalloc(elementCount*elementSize);
	SetMem(r, elementCount*elementSize, 0);
	return r;
}
FILE kfopen(CHAR16 *path, CHAR16 *mode, EFI_HANDLE *image){
	CHAR16 *pathcopy = StrDuplicate(path);
	CHAR16 *ptrpath = pathcopy;
	while(*ptrpath){
		if(*ptrpath == L'/')
			*ptrpath = L'\\';
		ptrpath++;
	}
	EFI_FILE_HANDLE ret = 0;
  EFI_LOADED_IMAGE *loaded_image = NULL;
	EFI_GUID lipGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	EFI_FILE_IO_INTERFACE *IOVolume;                       
	EFI_GUID fsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
	EFI_FILE_HANDLE vol;
  uefi_call_wrapper(BS->HandleProtocol, 3, image, &lipGuid, (void **) &loaded_image);
	uefi_call_wrapper(BS->HandleProtocol, 3,loaded_image->DeviceHandle, &fsGuid, (VOID*)&IOVolume);
	uefi_call_wrapper(IOVolume->OpenVolume, 2,IOVolume, &vol);
	if(StrCmp(mode, L"r") == 0)
		uefi_call_wrapper(vol->Open,5,vol, &ret, pathcopy, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);
	/*
	if(StrCmp(mode, L"w") == 0)
		uefi_call_wrapper(vol->Open,5,vol, &ret, path, EFI_FILE_MODE_WRITE, EFI_FILE_CREATE | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);
	if(StrCmp(mode, L"w+") == 0)
		uefi_call_wrapper(vol->Open,5,vol, &ret, path, EFI_FILE_MODE_READ, EFI_FILE_WRITE | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);
	*/

	kfree(pathcopy);
	return ret;
}
void kfclose(FILE f){
	uefi_call_wrapper(f->Close, 1, f);
}
size_t kfsize(FILE f){
	size_t ret = 0;
	EFI_FILE_INFO *FileInfo;
	FileInfo = LibFileInfo(f);
	ret = FileInfo->FileSize;
	kfree(FileInfo);
	return ret;
}
size_t kfread(void *buff, size_t szelement, size_t nbelement, FILE f){
	size_t ret = szelement*nbelement;
	uefi_call_wrapper(f->Read,1, f, &ret,buff);
	return ret;
}
