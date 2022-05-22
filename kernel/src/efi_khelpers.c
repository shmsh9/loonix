#include <efi_khelpers.h>

void *efi_allocate_pool(struct fnargs *fnargs, efi_uint_t sz){
    void *r = NULL;
    ((void __attribute__((ms_abi))(*)(enum efi_memory_type, efi_uint_t , void **))fnargs->SystemTable->boot->allocate_pool)(EFI_BOOT_SERVICES_DATA, sz, &r);
    return r;
}
