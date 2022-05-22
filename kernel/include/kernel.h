#ifndef KERNEL_H_
#define KERNEL_H_
#include <efi.h>
struct fnargs{
	efi_handle_t ImageHandle;
	EFI_SYSTEM_TABLE *SystemTable;
	CHAR16 *buff;
	CHAR16 *argv[16];
	char *charargv[256];
	unsigned int argc;
    void *printfn;
};
//#define printf(fmt, ..) (void __attribute__((ms_abi))(*)(fmt, ...))
#endif
