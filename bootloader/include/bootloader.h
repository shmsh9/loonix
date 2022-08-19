#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_
#include <efi.h>

struct framebuffer{
	uint16_t	height;
	uint16_t 	width;
	uintptr_t	address;
	uint64_t	size;
};
typedef struct{
	efi_handle_t     ImageHandle;
	EFI_SYSTEM_TABLE *SystemTable;
	efi_runtime_services *RuntimeServices;
	void             *kernelbase;
	void			 *kernelentry;
	uint64_t         kernelsize;
	struct efi_memory_descriptor *mmap;
	uint64_t mmap_size;
	uint64_t mmap_key;
	uint64_t uefi_exit_code;
	struct framebuffer framebuffer;
}bootinfo;
#endif

