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
    KDEBUG("ecam_base_address 0x%x", mcfg->ecam_base_address);
    KDEBUG("pci_start %d", mcfg->pci_bus_start);
    KDEBUG("pci_end %d", mcfg->pci_bus_end);
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
                pci_device_config *dev = (pci_device_config *)dev_config_address;
                if(dev->vendor_id == 0xffff)
                    continue;
                karray_push(pci_devices, (uint64_t)pci_device_ecam_new(dev_config_address, bus, slot, function));                

            }
        }
    }
}
uint64_t pci_ecam_dev_get_config_address(uint64_t ecam_base_address, uint16_t bus, uint8_t slot, uint8_t function){
    return (((bus * 256) + (slot * 8) + function) * 4096) + ecam_base_address;

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

void pci_bus_enum(uint64_t base){
    if(!base){
        KERROR("pci_config_space == 0x0");
        return;
    }
    pci_config_space = base;
    pci_config_data = pci_config_space+4;
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
	for(uint32_t bus = 0; bus < 256; bus++){
        for(uint32_t slot = 0; slot < 32; slot++){
            for(uint32_t function = 0; function < 8; function++){
                uint16_t vendor = pci_get_vendor_id(bus, slot, function);
                if(vendor == 0xffff)
                    continue;
                karray_push(pci_devices, (uint64_t)pci_device_new(bus, slot, function));                
            }
        }
    }
    KDEBUG("found %d pci devices", pci_devices->length);
}
void pci_device_write_data_32(pci_device *dev, uint16_t offset,uint32_t data){
    uint64_t address;
    uint64_t lbus = (uint64_t)dev->bus;
    uint64_t lslot = (uint64_t)dev->slot;
    uint64_t lfunc = (uint64_t)dev->function;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
            (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(pci_config_space, address);
    outl(pci_config_data, data);
}
void pci_device_write_data_16(pci_device *dev, uint16_t offset, uint16_t data){
    uint64_t address;
    uint64_t lbus = (uint64_t)dev->bus;
    uint64_t lslot = (uint64_t)dev->slot;
    uint64_t lfunc = (uint64_t)dev->function;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
            (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(pci_config_space, address);
    outw(pci_config_data, data);

}
uint32_t pci_device_read_data_32(pci_device *dev, uint16_t offset){
    uint64_t address;
    uint64_t lbus = (uint64_t)dev->bus;
    uint64_t lslot = (uint64_t)dev->slot;
    uint64_t lfunc = (uint64_t)dev->function;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(pci_config_space, address);
    return (uint32_t)((inl(pci_config_data) >> ((offset & 2) * 8)) & 0xffff);
}
uint16_t pci_device_read_data_16(pci_device *dev, uint16_t offset){
    uint64_t address;
    uint64_t lbus = (uint64_t)dev->bus;
    uint64_t lslot = (uint64_t)dev->slot;
    uint64_t lfunc = (uint64_t)dev->function;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(pci_config_space, address);
    return (uint16_t)((inw(pci_config_data) >> ((offset & 2) * 8)) & 0xffff);

}
uint16_t pci_read_16(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset){
	uint64_t address;
    uint64_t lbus = (uint64_t)bus;
    uint64_t lslot = (uint64_t)slot;
    uint64_t lfunc = (uint64_t)func;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(pci_config_space, address);
    return (uint16_t)((inl(pci_config_data) >> ((offset & 2) * 8)) & 0xffff);
}

pci_device *pci_device_new(uint16_t bus, uint16_t slot, uint16_t function){
    pci_device *ret = kcalloc(sizeof(pci_device), 1);
    *ret = (pci_device){
        .bus = bus,
        .slot = slot,
        .function = function,
        .vendor = pci_get_vendor_id(bus, slot, function),
        .product = pci_get_product_id(bus,slot, function),
        .class = pci_get_class_id(bus, slot, function),
        .subclass = pci_get_subclass_id(bus, slot, function)
    };
    return ret;
}

pci_device *pci_device_ecam_new(uint64_t address, uint32_t bus, uint8_t slot, uint8_t function){
    pci_device *ret = kcalloc(sizeof(pci_device), 1);
    pci_device_config *dev = (pci_device_config *)address;
    *ret = (pci_device){
        .bus = bus,
        .slot = slot,
        .function = function,
        .vendor = dev->vendor_id,
        .product = dev->product_id,
        .class = dev->class,
        .subclass = dev->subclass
    };
    return ret;

}
