#include <acpi.h>

acpi_table acpi_table_new(mmap *mmap){
    for(int i = 0; i < mmap->length; i++){
    struct efi_memory_descriptor *desc = (struct efi_memory_descriptor *)((uint64_t)mmap->mmap+(i*MMAP_ELEMENT_SIZE));
        if(desc->type == EFI_ACPI_MEMORY_NVS){
            return (acpi_table){
                .acpi_memory = desc->physical_start
            };
        }
    }
    KERROR("acpi table not found in mmap");
    return (acpi_table){0};
}
