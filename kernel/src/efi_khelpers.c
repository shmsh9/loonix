#include <efi_khelpers.h>

void *efi_allocate_pool(struct fnargs *fnargs, efi_uint_t sz){
    void *r = NULL;
    ((void __attribute__((ms_abi))(*)(enum efi_memory_type, efi_uint_t , void **))fnargs->SystemTable->boot->allocate_pool)(EFI_BOOT_SERVICES_DATA, sz, &r);
    return r;
}
void efi_free_pool(struct fnargs *fnargs, void *ptr){
    ((void __attribute__((ms_abi))(*)(void *))fnargs->SystemTable->boot->free_pool)(ptr);
}
void efi_print(struct fnargs *fnargs, CHAR16 *str){
    CHAR16 *cpstr = efi_allocate_pool(fnargs,256);
    CHAR16 *ptr = cpstr;
    while(*str){
        *ptr = *str;
        str++;
        ptr++;
    } 
    *ptr++ = 0;
    ((void __attribute__((ms_abi))(*)(struct efi_simple_text_output_protocol* ,CHAR16*))fnargs->SystemTable->out->output_string)(fnargs->SystemTable->out, str);
    efi_free_pool(fnargs, (void *)cpstr);
}
