#include <drivers/pci.h>
//https://github.com/levex/osdev/blob/master/drivers/pci/pci.c
karray *pci_devices = 0x0;
char *pci_class_strings[0x100] = {0};
uint64_t pci_config_space = 0x0;
uint64_t pci_config_data = 0x0;

void pci_enum_ecam(acpi_mcfg *mcfg){
    if(!mcfg){
        KERROR("MCFG == NULL");
        return;
    }
    if(!pci_devices){
        pci_devices = karray_new(sizeof(pci_device *), kfree);
    }
    else{
        karray_free(pci_devices);
        pci_devices = karray_new(sizeof(pci_device *), kfree);
    }
    if(!pci_devices){
        KERROR("error allocating mem for pci_devices");
        return;
    }
    pci_class_strings_init();

    for(uint16_t bus = 0; bus < mcfg->pci_bus_end; bus++){
        for(uint8_t slot = 0; slot < 32; slot++){
            for(uint8_t function = 0; function < 8; function++){
                uint64_t dev_config_address = pci_ecam_dev_get_config_address(mcfg->ecam_base_address, bus, slot, function);
                pci_config_header *dev = (pci_config_header *)dev_config_address;
                if(dev->vendor_id == 0xffff)
                    continue;
                karray_push(pci_devices, (uint64_t)pci_device_ecam_new(dev_config_address, bus, slot, function));                

            }
        }
    }
    KDEBUG("enumerated %d devices", pci_devices->length);
}
void pci_class_strings_init(){
    pci_class_strings[PCI_CLASS_MASS_STORAGE_CONTROLLER] = "Mass Storage Controller";
    pci_class_strings[PCI_CLASS_DISPLAY_CONTROLLER] = "Display Controller";
    pci_class_strings[PCI_CLASS_MULTIMEDIA_DEVICE] = "Multimedia Device";
    pci_class_strings[PCI_CLASS_MEMORY_CONTROLLER] = "Memory Controller";
    pci_class_strings[PCI_CLASS_BRIDGE_DEVICE] = "Bridge Device";
    pci_class_strings[PCI_CLASS_SIMPLE_COMMUNICATION_CONTROLLER] = "Simple Communication Controller";
    pci_class_strings[PCI_CLASS_BASE_SYSTEM_PERIPHERAL] = "Base System Peripheral";
    pci_class_strings[PCI_CLASS_INPUT_DEVICE] = "Input Device";
    pci_class_strings[PCI_CLASS_DOCKING_STATION] = "Docking Station";
    pci_class_strings[PCI_CLASS_PROCESSOR] = "Processor";
    pci_class_strings[PCI_CLASS_SERIAL_BUS_CONTROLLER] = "Serial Bus Controller";
    pci_class_strings[PCI_CLASS_UNASSIGNED] = "Unassigned class";

}

pci_device *pci_device_ecam_new(uint64_t address, uint32_t bus, uint8_t slot, uint8_t function){
    pci_device *ret = kcalloc(sizeof(pci_device), 1);
    pci_config_header *dev_h = (pci_config_header *)address;
    *ret = (pci_device){
        .bus = bus,
        .slot = slot,
        .function = function,
        .header = dev_h,
        .dev0 = 0x0,
        .dev1 = 0x0,
        .dev2 = 0x0
    };
    return ret;

}

uint8_t pci_device_get_header_type(pci_device *dev){
    uint8_t ret = dev->header->header_type;
    unset_bit(&ret, 7);
    return ret;
}
