#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_
#include <efi.h>
#define K_STACK_SIZE 0x400000

struct framebuffer{
	uint16_t	height;
	uint16_t 	width;
	uintptr_t	address;
	uint64_t	size;
};
struct bootinfo{
	efi_handle_t     ImageHandle;
	EFI_SYSTEM_TABLE *SystemTable;
	efi_runtime_services *RuntimeServices;
	void             *kernelbase;
	void			 *kernelentry;
	uint64_t         kernelsize;
	struct efi_memory_descriptor *mmap;
	struct framebuffer framebuffer;
};
#endif

