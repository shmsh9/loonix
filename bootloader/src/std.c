#include <std.h>

void *kmalloc(size_t sz){
	void *r = NULL;
	EFI_STATUS s = SystemTable->BootServices->AllocatePool(EfiLoaderData, sz, &r);
	if(EFI_ERROR(s)){
		Print(L"error : kmalloc() : 0x%x\n", s);
		return NULL;
	}
	return r;
}
void *kcalloc(size_t elementCount, size_t elementSize){
	void *r = kmalloc(elementCount*elementSize);
	SetMem(r, elementCount*elementSize, 0);
	return r;
}
void *kallocaddress(size_t sz, void *address){
		EFI_STATUS s = SystemTable->BootServices->AllocatePages(AllocateAddress, EfiBootServicesData, EFI_SIZE_TO_PAGES(sz), address);
		if(EFI_ERROR(s))
			Print(L"error : kallocaddress : 0x%x\n", s);
		return address;
}
void kfree(void *ptr){
	SystemTable->BootServices->FreePool(ptr);
}
FILE * kfopen(CHAR16 *path, CHAR16 *mode, EFI_HANDLE *image){
	CHAR16 *pathcopy = StrDuplicate(path);
	CHAR16 *ptrpath = pathcopy;
	while(*ptrpath){
		if(*ptrpath == L'/')
			*ptrpath = L'\\';
		ptrpath++;
	}
	
	EFI_FILE_PROTOCOL *ret = NULL;
  	EFI_LOADED_IMAGE_PROTOCOL *loaded_image = NULL;
	EFI_GUID lipGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *IOVolume = NULL;                       
	EFI_GUID fsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
	EFI_FILE_PROTOCOL *vol;
	SystemTable->BootServices->OpenProtocol(image, &lipGuid, (void **)&loaded_image, image, NULL,EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
	SystemTable->BootServices->OpenProtocol(loaded_image->DeviceHandle,  &fsGuid, (void **)&IOVolume, loaded_image->DeviceHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
	IOVolume->OpenVolume(IOVolume, &vol);
	if(StrCmp(mode, L"r") == 0)
		vol->Open(vol, &ret, pathcopy, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);
		//uefi_call_wrapper(vol->Open,5,vol, &ret, pathcopy, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);
	/*
	if(StrCmp(mode, L"w") == 0)
		uefi_call_wrapper(vol->Open,5,vol, &ret, path, EFI_FILE_MODE_WRITE, EFI_FILE_CREATE | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);
	if(StrCmp(mode, L"w+") == 0)
		uefi_call_wrapper(vol->Open,5,vol, &ret, path, EFI_FILE_MODE_READ, EFI_FILE_WRITE | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);
	*/

	kfree(pathcopy);
	return ret;
}
void kfclose(FILE *f){
	f->Close(f);
}
size_t kfsize(FILE *f){
	uint64_t ret = 0;
	EFI_FILE_INFO *FileInfo = NULL;
	UINTN size = 0;
	EFI_GUID guid = EFI_FILE_INFO_ID;
	f->GetInfo(f, &guid, &size, NULL);
	FileInfo = kcalloc(size,1);
	f->GetInfo(f, &guid, &size, FileInfo);
	ret = FileInfo->FileSize;
	kfree(FileInfo);
	return ret;
}
size_t kfread(void *buff, size_t szelement, size_t nbelement, FILE *f){
	size_t ret = szelement*nbelement;
	f->Read(f, &ret,buff);
	return ret;
}

void CopyMem(void *dst, void *src, uint64_t sz){
	for(uint64_t i = 0; i < sz; i++)
		((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
}
uint64_t StrCmp(CHAR16 *s1, CHAR16 *s2){
	while(*s1 && *s2){
		if(*s1 != *s2)
			return (uint64_t)s1;
		s1++;
		s2++;
	}
	return 0;
}
uint64_t strlen(CHAR16 *str){
	uint64_t r = 0;
	while (*str){
		r++;
		str++;
	}
	return r;
}
void SetMem(void *dst, uint8_t b, uint64_t sz){
	for(uint64_t i = 0; i < sz; i++)
		((uint8_t *)(dst))[i] = b;
}

void Print(CHAR16 *fmt, ...){
	__builtin_va_list args;
	__builtin_va_start(args, fmt);
	uint64_t l = strlen(fmt);
	CHAR16 hexlow[] = L"0123456789abcdef";
	CHAR16 hexupp[] = L"0123456789ABCDEF";
	for(int i = 0; i < l; i++){
		if(fmt[i] == L'%' && i+1 < l){
			if(fmt[i+1] == L's'){
				CHAR16 *ptrstr = __builtin_va_arg(args, CHAR16*);
				SystemTable->ConOut->OutputString(SystemTable->ConOut,ptrstr);
				i += 2;
			}
			if(fmt[i+1] == L'd'){
				CHAR16 tmpnum[64] =  {0};
				CHAR16 *ptrtmpnum = tmpnum;
				ptrtmpnum = tmpnum;
				uint32_t num = __builtin_va_arg(args, uint32_t);
				int c = 0;
				if(!num)
					tmpnum[0] = L'0';
				while(num){
					*ptrtmpnum = hexlow[num % 10] ;
					ptrtmpnum++;
					c++;
					num /= 10;	
				}
				int right = c-1;
				int left = 0;
				while(left < right){
					CHAR16 t = tmpnum[right];
					tmpnum[right] = tmpnum[left];
					tmpnum[left] = t;
					left++;
					right--;
				}
				SystemTable->ConOut->OutputString(SystemTable->ConOut,tmpnum);
				i += 2;
			}
			if(fmt[i+1] == L'x' || fmt[i+1] == L'X'){
				CHAR16 tmpnum[64] =  {0};
				CHAR16 *ptrtmpnum = tmpnum;
				uint32_t num = __builtin_va_arg(args, uint32_t);
				int c = 0;
				if(!num)
					tmpnum[0] = L'0';
				while(num){
						*ptrtmpnum = fmt[i+1] == L'x' ? hexlow[num % 16] : hexupp[num %16];
						ptrtmpnum++;
						c++;
						num /= 16;
				}
				int right = c-1;
				int left = 0;
				while(left < right){
					CHAR16 t = tmpnum[right];
					tmpnum[right] = tmpnum[left];
					tmpnum[left] = t;
					left++;
					right--;
				}
				SystemTable->ConOut->OutputString(SystemTable->ConOut,tmpnum);
				i += 2;
			}
		}
		CHAR16 endchar[2] = {fmt[i], 0};
		if(fmt[i] == L'\n')
			SystemTable->ConOut->OutputString(SystemTable->ConOut,L"\r");

		SystemTable->ConOut->OutputString(SystemTable->ConOut,endchar);
	}
	__builtin_va_end(args);
}

CHAR16 *StrDuplicate(CHAR16 *str){
	uint64_t l = strlen(str);
	CHAR16 *ret = kmalloc(l+1);
	for (uint64_t i = 0; i < l+1; i++){
		ret[i] = str[i];
	}
	return ret;
}

void * memset(void *dst, char b, size_t sz){
	for(size_t i = 0; i < sz; i++)
		((char *)dst)[i] = b;
	return dst;
}

void *memcpy(void *dst, const void * src, size_t sz){
	for(size_t i = 0; i < sz; i++)
		((char *)dst)[i] = ((char *)src)[i];
	return dst;
}
