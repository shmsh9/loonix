#ifndef EFI_KHELPERS_H
#define EFI_KHELPERS_H
#include <efi.h>
#include <kernel.h>
//can only print efi allocated memory
//void __attribute((ms_abi))(*__efi_printfn)(CHAR16 *, ...) = (void __attribute__((ms_abi))(*)(CHAR16 *, ...))fnargs->printfn;
void *efi_allocate_pool(struct fnargs *fnargs, efi_uint_t sz);
#endif
