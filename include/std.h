#ifndef IO_H_
#define IO_H_

#define free(ptr) FreePool(ptr)
#ifdef	__amd64__
typedef unsigned long long size_t;
#endif
#ifdef __aarch64__
typedef unsigned long size_t;
#endif
typedef BOOLEAN bool;
typedef EFI_FILE_HANDLE FILE;

void *malloc(size_t sz){
	void *r = 0x0;
	uefi_call_wrapper(gBS->AllocatePool,3,EfiBootServicesData, sz, &r);
	if(!r){
		Print(L"malloc() : failure\n");
	}
	return r;
}
void *calloc(size_t elementCount, size_t elementSize){
	void *r = malloc(elementCount*elementSize);
	SetMem(r, elementCount*elementSize, 0);
	return r;
}
FILE fopen(CHAR16 *path, CHAR16 *mode, EFI_HANDLE *image){
	EFI_FILE_HANDLE ret = 0;
  EFI_LOADED_IMAGE *loaded_image = NULL;
	EFI_GUID lipGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	EFI_FILE_IO_INTERFACE *IOVolume;                       
	EFI_GUID fsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
	EFI_FILE_HANDLE vol;
  uefi_call_wrapper(BS->HandleProtocol, 3, image, &lipGuid, (void **) &loaded_image);
	uefi_call_wrapper(BS->HandleProtocol, 3,loaded_image->DeviceHandle, &fsGuid, (VOID*)&IOVolume);
	uefi_call_wrapper(IOVolume->OpenVolume, 2,IOVolume, &vol);
	uefi_call_wrapper(vol->Open,5,vol, &ret, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY| EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);
	return ret;
}
void fclose(FILE f){
	uefi_call_wrapper(f->Close, 1, f);
}
size_t fsize(FILE f){
	size_t ret = 0;
	EFI_FILE_INFO *FileInfo;
	FileInfo = LibFileInfo(f);
	ret = FileInfo->FileSize;
	free(FileInfo);
	return ret;
}
size_t fread(void *buff, size_t szelement, size_t nbelement, FILE f){
	size_t ret = szelement*nbelement;
	uefi_call_wrapper(f->Read,1, f, &ret,buff);
	return ret;
}
#endif
