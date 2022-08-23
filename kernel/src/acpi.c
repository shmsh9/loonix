#include <acpi.h>

acpi_table *acpi_table_new(bootinfo *bootinfo){
    if(!bootinfo->acpi_table){
        KERROR("acpi table not found in mmap");
        return 0x0;
    }
    return bootinfo->acpi_table;
}

void acpi_table_debug_print(acpi_table *table){
    if(!table)
        return;
    kprint("ACPI_TABLE : \n");
    kprinthex((void *)table->VendorTable, sizeof(acpi_sdt_header));
} 

// 0xb0 0xcd 0x1b 0xfc 0x31 0x7d 0xaa 0x49
// 0x98 0x58 0x4e 0xee 0x14 0x39 0x59 0x42
