#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_
#include <efi.h>

struct bootinfo{
	efi_handle_t ImageHandle;
	EFI_SYSTEM_TABLE *SystemTable;
	void *stackptr;
	void *kernelbegin;
	void *kernelend;
};

#endif

