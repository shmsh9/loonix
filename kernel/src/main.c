#include <kernel.h>
#include <efi_khelpers.h>

uint64_t main(struct fnargs *fnargs){
    //fnargs->SystemTable->out->output_string(fnargs->SystemTable->out, fnargs->argv[0]);
    //allocate_pool(EFI_BOOT_SERVICES_DATA, 6, (void **)&buff);
    
    //printf(L"%s", L"Hello World !\n");
    //return 0xdeadcaca;
    return (uint64_t)efi_allocate_pool(fnargs, 4096);
}

