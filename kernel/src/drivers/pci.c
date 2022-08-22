#include <drivers/pci.h>
//https://github.com/levex/osdev/blob/master/drivers/pci/pci.c
karray *pci_devices = 0x0;

void pci_bus_enum(){
    if(!PCI_BUS_ADDRESS){
        KERROR("PCI_BUS_ADDRESS == 0x0");
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

uint16_t pci_read_16(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset){
	uint64_t address;
    uint64_t lbus = (uint64_t)bus;
    uint64_t lslot = (uint64_t)slot;
    uint64_t lfunc = (uint64_t)func;
    uint16_t tmp = 0;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(PCI_BUS_ADDRESS, address);
    tmp = (uint16_t)((inl(PCI_BUS_CONFIG) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
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
