#include <drivers/pci.h>
//https://github.com/levex/osdev/blob/master/drivers/pci/pci.c
karray *pci_devices = 0x0;
char *pci_class_strings[0x100] = {0};

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

void pci_bus_enum(){
    /*
    if(!PCI_BUS_ADDRESS){
        KERROR("PCI_BUS_ADDRESS == 0x0");
        return;
    }
    */
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
    outl(PCI_BUS_ADDRESS, address);
    outl(PCI_CONFIG_DATA, data);
}
void pci_device_write_data_16(pci_device *dev, uint16_t offset, uint16_t data){
    uint64_t address;
    uint64_t lbus = (uint64_t)dev->bus;
    uint64_t lslot = (uint64_t)dev->slot;
    uint64_t lfunc = (uint64_t)dev->function;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
            (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(PCI_BUS_ADDRESS, address);
    outw(PCI_CONFIG_DATA, data);

}
uint32_t pci_device_read_data_32(pci_device *dev, uint16_t offset){
    uint64_t address;
    uint64_t lbus = (uint64_t)dev->bus;
    uint64_t lslot = (uint64_t)dev->slot;
    uint64_t lfunc = (uint64_t)dev->function;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(PCI_BUS_ADDRESS, address);
    return (uint32_t)((inl(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
}
uint16_t pci_device_read_data_16(pci_device *dev, uint16_t offset){
    uint64_t address;
    uint64_t lbus = (uint64_t)dev->bus;
    uint64_t lslot = (uint64_t)dev->slot;
    uint64_t lfunc = (uint64_t)dev->function;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(PCI_BUS_ADDRESS, address);
    return (uint16_t)((inw(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);

}
uint16_t pci_read_16(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset){
	uint64_t address;
    uint64_t lbus = (uint64_t)bus;
    uint64_t lslot = (uint64_t)slot;
    uint64_t lfunc = (uint64_t)func;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(PCI_BUS_ADDRESS, address);
    return (uint16_t)((inl(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
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
