#include <acpi.h>
acpi_xsdt * acpi_table_new(bootinfo *bi){
    acpi_rsd_ptr *p = acpi_find_rsd_ptr(bi->acpi_table, bi->acpi_table_length);
    KDEBUG("xsdt_address 0x%x", p->XsdtAddress);
    acpi_sdt_header *xsdt = (acpi_sdt_header *)p->XsdtAddress;
    if(*(uint32_t *)xsdt->signature != *(uint32_t *)"XSDT"){
        KERROR("XSDT structure is invalid");
        return 0x0;
    }
    uint64_t checksum = 0;
    uint8_t *xsdt_casted = (uint8_t *)xsdt;
    for(uint16_t i = 0; i < xsdt->length; i++)
        checksum += xsdt_casted[i];
    if(checksum & 0xff){
        KERROR("xsdt checksum failed last byte != 0x00 0b%b", checksum);
        return 0x0;
    }
    return (acpi_xsdt *)xsdt_casted;
}
acpi_sdt_header *acpi_find_fadt(acpi_xsdt *xsdt){
    uint16_t entries = xsdt->header.length - sizeof(acpi_sdt_header);
    for(uint16_t i = 0; i < entries; i++){
        if(*(uint32_t *)xsdt->tables[i].signature == *(uint32_t *)"FACP"){
            return acpi_sdt_valid_checksum(xsdt->tables+i) ? xsdt->tables+i : 0x0;
        }
    }
    KERROR("FADT table not found");
    return 0x0;
}
bool acpi_sdt_valid_checksum(acpi_sdt_header *h){
    uint64_t checksum = 0;
    uint8_t *h_casted = (uint8_t *)h;
    for(uint16_t i = 0; i < h->length; i++)
        checksum += h_casted[i];
    if(checksum & 0xff){
        KERROR("sdt checksum failed last byte != 0x00 0b%b", checksum);
        return false;
    }
    return true;

}
void acpi_table_debug_print(acpi_xsdt *table){
    uint16_t entries = table->header.length - sizeof(acpi_sdt_header);
    for(uint16_t i = 0; i < entries; i++){
        kprinthex((void *)(table->tables+i), sizeof(acpi_rsd_ptr));
    }
}
void * acpi_find_rsd_ptr(EFI_CONFIGURATION_TABLE *table, uint64_t ntable){
    if(!table)
        return 0x0;
    for(int i = 0; i < ntable; i++){
        if(*(uint64_t *)table[i].VendorTable == *(uint64_t *)"RSD PTR "){
            KDEBUG("found RSD PTR 0x%x", table[i].VendorTable);
            uint8_t *casted_table = (uint8_t *)table[i].VendorTable;
            uint64_t checksum = 0;
            //acpi_rsd_ptr *tmp = table[i].VendorTable;
            //20 == first header part
            for(uint32_t i = 0; i < 20; i++){
                checksum += casted_table[i];
            }
            if(checksum & 0xff){
                KERROR("RSD PTR checksum failed last byte != 0x00 0b%b", checksum);
                continue;
            }
            checksum = 0;
            for(uint32_t i = 20; i < sizeof(acpi_rsd_ptr); i++){
                checksum += casted_table[i];
            }
            if(checksum & 0xff){
                KERROR("RSD PTR checksum failed last byte != 0x00 0b%b", checksum);
                continue;
            }
            return table[i].VendorTable;
        }
    }
    return 0x0;
}
// 0xb0 0xcd 0x1b 0xfc 0x31 0x7d 0xaa 0x49
// 0x98 0x58 0x4e 0xee 0x14 0x39 0x59 0x42
