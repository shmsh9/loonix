#ifndef EXTERN_H_
#define EXTERN_H_
extern EFI_HANDLE ImageHandle;
extern EFI_SYSTEM_TABLE *SystemTable;
extern EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
extern struct stack *usralloc;
#endif
