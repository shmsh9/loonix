#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_
#include <efi.h>
#define K_STACK_SIZE 0x400000
struct bootinfo{
	efi_handle_t     ImageHandle;
	EFI_SYSTEM_TABLE *SystemTable;
	void             *kernelbase;
	void						 *kernelentry;
	uint64_t         kernelsize;
	uint64_t         stacksize;
};

#endif

