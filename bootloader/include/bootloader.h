#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_
#include <Uefi.h>

struct framebuffer{
	uint16_t	height;
	uint16_t 	width;
	uintptr_t	address;
	uint64_t	size;
};
typedef struct{
	EFI_HANDLE     ImageHandle;
	EFI_SYSTEM_TABLE *SystemTable;
	EFI_RUNTIME_SERVICES *RuntimeServices;
	void             *kernelbase;
	void			 *kernelentry;
	uint64_t         kernelsize;
	EFI_MEMORY_DESCRIPTOR *mmap;
	uint64_t mmap_size;
	uint64_t mmap_key;
	uint64_t uefi_exit_code;
	EFI_CONFIGURATION_TABLE *acpi_table;
	struct framebuffer framebuffer;
}bootinfo;
#endif

