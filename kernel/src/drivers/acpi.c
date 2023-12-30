#include <drivers/acpi.h>
acpi_table *acpi_table_new(bootinfo *bi){
    acpi_rsd_ptr *rsd_ptr = acpi_find_rsd_ptr(bi->acpi_table, bi->acpi_table_length);
    acpi_xsdt *xsdt = acpi_find_xsdt(rsd_ptr);
    if(!xsdt){
        KERROR("XSDT not found");
        return 0x0;
    }
    acpi_fadt *fadt = (acpi_fadt *)acpi_find_table(xsdt, "FACP");
    if(!fadt){
        KERROR("FADT not found");
        return 0x0;
    }
    acpi_mcfg *mcfg = (acpi_mcfg *)acpi_find_table(xsdt, "MCFG");
    if(!mcfg){
        KERROR("MCFG not found");
        return 0x0;
    }
    acpi_table *ret = kcalloc(sizeof(acpi_table), 1);
    *ret = (acpi_table){
        .fadt = fadt,
        .xsdt = xsdt,
        .mcfg = mcfg
    };
    return ret;
}
acpi_sdt_header *acpi_find_table(acpi_xsdt *xsdt,char signature[4]){
    uint16_t entries = (xsdt->header.length - sizeof(xsdt->header)) / 8;
    for(uint16_t i = 0; i < entries; i++){
        acpi_sdt_header *tmp_h = (acpi_sdt_header *)(xsdt->tables[i]);
        if(memcmp(tmp_h->signature,signature, 4) == 0){
            if(acpi_sdt_valid_checksum(tmp_h))
                return tmp_h;
            continue;

        }
    }
    //char signature_str[5] = {signature[0],signature[1],signature[2],signature[3], 0 };
    //KERROR("%s table not found", signature_str);
    return 0x0;

}
acpi_xsdt *acpi_find_xsdt(acpi_rsd_ptr *rsd_ptr){
    acpi_sdt_header *xsdt = (acpi_sdt_header *)rsd_ptr->XsdtAddress;
    if(*(uint32_t *)xsdt->signature != *(uint32_t *)"XSDT"){
        KERROR("XSDT structure is invalid");
        return 0x0;
    }
    return acpi_sdt_valid_checksum(xsdt) ? (acpi_xsdt *)xsdt : 0x0;
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
    uint16_t entries = (table->header.length - sizeof(table->header)) / 8;
    KDEBUG("entries : %d", entries);
    for(uint16_t i = 0; i < entries; i++){
        kprinthex((void *)(table->tables[i]), sizeof(acpi_sdt_header));

        if(!acpi_sdt_valid_checksum((acpi_sdt_header *)table->tables[i]))
            continue;
        /*
        char sign[] = {
           (acpi_sdt_header *)table->tables[i]).signature[0],
            table->tables[i].signature[1], 
            table->tables[i].signature[2],
            table->tables[i].signature[3],
            0
        };
        */
    }
}
acpi_rsd_ptr *acpi_find_rsd_ptr(EFI_CONFIGURATION_TABLE *table, uint64_t ntable){
    if(!table)
        return 0x0;
    for(int i = 0; i < ntable; i++){
        if(*(uint64_t *)table[i].VendorTable == *(uint64_t *)"RSD PTR "){
            uint8_t *casted_table = (uint8_t *)table[i].VendorTable;
            uint64_t checksum = 0;
            //acpi_rsd_ptr *tmp = table[i].VendorTable;
            //20 == first header part
            for(uint32_t i = 0; i < 20; i++){
                checksum += casted_table[i];
            }
            if(checksum & 0xff){
                //KERROR("RSD PTR checksum failed last byte != 0x00 0b%b", checksum);
                continue;
            }
            checksum = 0;
            for(uint32_t i = 20; i < sizeof(acpi_rsd_ptr); i++){
                checksum += casted_table[i];
            }
            if(checksum & 0xff){
                //KERROR("RSD PTR checksum failed last byte != 0x00 0b%b", checksum);
                continue;
            }
            return (acpi_rsd_ptr *)table[i].VendorTable;
        }
    }
    KERROR("no valid entry found");
    return 0x0;
}
// 0xb0 0xcd 0x1b 0xfc 0x31 0x7d 0xaa 0x49
// 0x98 0x58 0x4e 0xee 0x14 0x39 0x59 0x42
