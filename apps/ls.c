#include <stdlib.h>
#include <stdio.h>
int main(int argc, CHAR16 **argv){
	EFI_FILE_PROTOCOL *RootDir;
  UINT8 Buffer[1024];
  UINTN BufferSize;
  EFI_FILE_INFO *FileInfo;
  uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &RootDir);
  while(1){
  	BufferSize = sizeof(Buffer);
   	uefi_call_wrapper(RootDir->Read, 3, RootDir, &BufferSize, Buffer);
   	if (BufferSize == 0) {
   		break;
   	}
		FileInfo = (EFI_FILE_INFO *)Buffer;
   	printf(FileInfo->FileName);
    printf(L"\n");
	}
  uefi_call_wrapper(RootDir->Close, 1, RootDir);
	return 0;
}
