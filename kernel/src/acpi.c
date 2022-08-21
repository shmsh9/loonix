#include <acpi.h>

acpi_table acpi_table_new(mmap *mmap){
    for(int i = 0; i < mmap->length; i++){
    EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)((uint64_t)mmap->mmap+(i*MMAP_ELEMENT_SIZE));
        if(desc->Type == EFI_ACPI_MEMORY_NON_WRITABLE){
            return (acpi_table){
                .acpi_memory = desc->PhysicalStart
            };
        }
    }
    KERROR("acpi table not found in mmap");
    return (acpi_table){0};
}
